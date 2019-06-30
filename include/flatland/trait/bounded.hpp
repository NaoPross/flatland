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
        bool are_colliding(const T&, const U&);

        // overload reverses in terms of the first
        template<typename T, typename U>
        bool are_colliding(const T& t, const U& u)
        {
            return are_colliding(u, t);
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
            virtual bool collides(std::shared_ptr<const any>) const = 0;
            virtual bool collides(const rectangle&) const = 0;
            virtual bool collides(const circle&) const = 0;
            virtual bool collides(const rectangles&) const = 0;
        };


        /// helper structure that uses CRTP to inject collision overloads
        template<typename T>
        struct make_bound : public any
        {
            // collision with itself need to be implemented
            virtual bool collides(const T&) const override = 0;

            // collision with others, is implemented outside
            template<typename U,
                     // note: U cannot be T or any
                     typename std::enable_if_t<
                         std::disjunction_v<
                             std::negation_v<std::is_same_v<U, T>>,
                             std::negation_v<std::is_same_v<U, any>>
                      >>>
            bool collides(const U& other) const
            {
                return are_colliding(static_cast<T>(*this), other);
            }

            // collision with pointer to any bound
            bool collides(std::shared_ptr<const any> a) const override
            {
                // calls the collision of the other class by
                // statically downcasting, because here the type is
                // known
                return a->collides(static_cast<const T&>(*this));
            }
        };


        /* bound types
         *
         * for each new bound type, for example named F, that may need to be
         * implemented, requirements are:
         *
         * - inherit publicly make_bound<F>
         * - add a member "collides(const F&) const = 0" to bound::any
         * - the bound may need to be friend with are_colliding(T, U)
         * - one must write overloads of are_colliding(F, U) where U
         *   is every other bound type
         *
         */

        class rectangle : public make_bound<rectangle>
        {
        public: 
            template<typename T, typename U>
            friend bool bound::are_colliding(const T&, const U&);

            rectangle(wsdl2::rect r) : m_rect(r) {}

            bool collides(const rectangle& other) const override;
            wsdl2::rect enclosing_rect() const override { return m_rect; }

        private:
            wsdl2::rect m_rect;
        };

        class circle : public make_bound<circle>
        {
        public:
            template<typename T, typename U>
            friend bool are_colliding(const T&, const U&);

            circle(mm::vec2<int> pos, unsigned radius)
                : m_pos(pos), m_radius(radius) {}

            bool collides(const circle& other) const override;
            wsdl2::rect enclosing_rect() const override;

        private:
            mm::vec2<int> m_pos;
            unsigned m_radius;
        };

        class rectangles : public make_bound<rectangles>
        {
        public:
            template<typename T, typename U>
            friend bool bound::are_colliding(const T&, const U&);

            bool collides(const rectangles& other) const override;
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
            virtual bool collides(const bounded& other) const {
                return other.bound()->collides(bound());
            }
        };
    }
}
