#pragma once

#include <mm/mmvec.hpp>
#include <wsdl2/video.hpp>

#include <type_traits>
#include <utility>
#include <set>


namespace flat
{
    namespace bound
    {
        template<typename T, typename U>
        bool are_overlapping(const T&, const U&);

        // overload reverses in terms of the first
        template<typename T, typename U>
        bool are_overlapping(const T& t, const U& u)
        {
            return are_overlapping(u, t);
        }

        class rectangle;
        class circle;
        class rectangles;

        class any
        {
        public:
            virtual ~any() = default;

            /// get enclosing rectangle
            virtual wsdl2::rect enclosing_rect() const = 0;

            /// define all possible collisions
            virtual bool overlaps(std::shared_ptr<const any>) const = 0;
            virtual bool overlaps(const rectangle&) const = 0;
            virtual bool overlaps(const circle&) const = 0;
            virtual bool overlaps(const rectangles&) const = 0;
        };


        /// helper structure that uses CRTP to inject collision overloads
        template<typename T>
        struct make_bound : public any
        {
            // check if overlaps with itself need to be implemented
            virtual bool overlaps(const T&) const override = 0;

            // overlap with others is implemented outside
            template<typename U,
                     // note: U cannot be T or any
                     typename std::enable_if_t<
                         std::disjunction_v<
                             std::negation_v<std::is_same_v<U, T>>,
                             std::negation_v<std::is_same_v<U, any>>
                      >>>
            bool overlaps(const U& other) const
            {
                return are_overlapping(static_cast<T>(*this), other);
            }

            // overlap with pointer to any bound
            bool overlaps(std::shared_ptr<const any> a) const override
            {
                // calls the ovelap check of the other class by
                // statically downcasting, because here the type is
                // known
                return a->overlaps(static_cast<const T&>(*this));
            }
        };


        /* bound types
         *
         * for each new bound type, for example named F, that may need to be
         * implemented, requirements are:
         *
         * - inherit publicly make_bound<F>
         * - add a member "overlaps(const F&) const = 0" to bound::any
         * - the bound may need to be friend with are_overlapping(T, U)
         * - one must write overloads of are_overlapping(F, U) where U
         *   is every other bound type
         *
         */

        class rectangle : public make_bound<rectangle>
        {
        public: 
            template<typename T, typename U>
            friend bool bound::are_overlapping(const T&, const U&);

            rectangle(wsdl2::rect r) : m_rect(r) {}

            bool overlaps(const rectangle& other) const override;
            wsdl2::rect enclosing_rect() const override { return m_rect; }

        private:
            wsdl2::rect m_rect;
        };

        class circle : public make_bound<circle>
        {
        public:
            template<typename T, typename U>
            friend bool are_overlapping(const T&, const U&);

            circle(mm::vec2<int> pos, unsigned radius)
                : m_pos(pos), m_radius(radius) {}

            bool overlaps(const circle& other) const override;
            wsdl2::rect enclosing_rect() const override;

        private:
            mm::vec2<int> m_pos;
            unsigned m_radius;
        };

        class rectangles : public make_bound<rectangles>
        {
        public:
            template<typename T, typename U>
            friend bool bound::are_overlapping(const T&, const U&);

            bool overlaps(const rectangles& other) const override;
            wsdl2::rect enclosing_rect() const override;

        private:
            std::multiset<wsdl2::rect> m_rects;
        };
    }

    namespace trait
    {
        struct bounded
        {
            virtual ~bounded() = default;
            virtual std::shared_ptr<const bound::any> bound() const = 0;
        };

        struct solid : public bounded
        {
            virtual bool collides(const solid& other) const {
                return other.bound()->overlaps(bound());
            }
        };
    }

    /*
    namespace helper
    {
        
        
        
        
    }

    class ebox;
    struct ebox::node;

    // enclosing box, collision check region
    class ebox : private std::multiset<node>
    {
        struct node
        {
            // get the enclosing rectangle and find the index approximation
            //void update();

            unsigned int x;
            unsigned int y;    
            std::weak_ptr<bounded> object;

            inline bool operator<(const node& other) const {
                // check y first, then if equal, check x
                return this->y < other.y || (this->y == other.y && this->x < other.x);
            }
            
            // create a pair of nodes from the corners of the bound's enclosing rectangle
            static std::pair<node, node> create(std::shared_ptr<bounded>);

        private:
            node(unsigned int _x, unsigned int _y, std::shared_ptr<bounded> _obj)
                : x(_x), y(_y), object(_object) {}
        };

    public:

        ebox(double _length, 
             double _height, 
             unsigned char _depth_x, 
             unsigned char _depth_y, 
             const mm::vec2<double>& _location, 
             std::initializer_list<std::shared_ptr<bounded>> init = {});

        void insert(std::shared_ptr<bounded>);
        void erase(std::shared_ptr<bounded>);

        using std::multiset<enode>::size;
        using std::multiset<enode>::empty;
        using std::multiset<enode>::clear;

        using std::multiset<enode>::begin;
        using std::multiset<enode>::cbegin;
        using std::multiset<enode>::end;
        using std::multiset<enode>::cend;

        using std::multiset<enode>::rbegin;
        using std::multiset<enode>::crbegin;
        using std::multiset<enode>::rend;
        using std::multiset<enode>::crend; 
        
        const double length;
        const double height;

        // power of 2 which determines the rectangle discretisation
        const unsigned char depth_x;
        const unsigned char depth_y;

        const mm::vec2<double> location;

        // upair comparer
        template<class Pair>
        struct upair_comp 
        {
            bool operator()(const Pair& lhs, const Pair& rhs) const
            {
                return lhs == rhs || (lhs.first == rhs.second && lhs.second == rhs.first);
            }
        };

        // unordered pair
        template<class T>
        struct upair_set : public std::unordered_set<std::pair<T, T>, 
                                                     std::hash<std::pair<T, T>>, 
                                                     helper::upair_comp<std::pair<T, T>>> 
        {
            using std::unordered_set<std::pair<T, T>, std::hash<std::pair<T, T>>, helper::upair_comp<T>>::unordered_set;

            upair_set& operator<<(const upair_set& rhs) {
                insert(rhs.begin(), rhs.end());
                return *this;
            }
        };

        inline upair_set<std::weak_ptr<bounded>> check_collisions() const {
            return check_collision(this->begin(), this->end(), );
        }


    private:

        typedef std::multiset<node>::const_iterator const_iterator;

        static upair_set<std::weak_ptr<bounded>> check_collisions(const_iterator down, const_iterator up, const std::size_t depth) const;
    };

    // get all overlapping, O(N log(N)) algorithm
    // where N is the size of L
    template<class T,
             template<class> class List, 
             class Rect = wsdl2::rect>
    upair_set<T> overlapping_pairs(const List<T>& L, const Rect& domain)
    {
        const size_t N = L.size();
        if (N == 0 || N == 1)
            return upair_set(); // empty set
        else if (N == 2)
            return upair_set({std::make_pair<T, T>(L[0], L[1])});

        const Rect b_left{domain.x, domain.y, domain.w/2, domain.h/2};
        const Rect b_right{domain.x + domain.w/2, domain.y, domain.w/2, domain.h/2};
        const Rect u_left{domain.x, domain.y + domain.h/2, domain.w/2, domain.h/2};
        const Rect u_right{domain.x + domain.w/2, domain.y + domain.h/2, domain.w/2, domain.h/2};

        return overlapping_pairs(next_b_left, b_left) <<
               overlapping_pairs(next_b_right, b_right) <<
               overlapping_pairs(next_u_left, u_left) <<
               overlapping_pairs(next_u_right, u_right);
    }*/
}
