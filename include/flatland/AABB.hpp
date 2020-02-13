#pragma once

#include "flatland/trait/bounded.hpp"
#include "flatland/core/geometry.hpp"

// Axys aligned bounding box interface
namespace flat::AABB {

    // forward declarations
    class node;
    class branch;
    class leaf;
    class tree;
    
    using rect = flat::geom::rect<double>;

    // node of the tree
    struct node {

        node(const rect& box, branch* parent = nullptr) 
            : m_box(box), m_parent(parent) {}

        virtual ~node() {}
        
        // rectangle bounds, lower = bottom-left corner, upper = top-right corner
        inline const mm::vec2<double>& lower_bound() const {
            return m_box.lower;
        }
        inline const mm::vec2<double>& upper_bound() const {
            return m_box.upper;
        }

        // check whether it's a leaf or a branch
        virtual bool is_leaf() const = 0;

        enum overlap {
            inside, partial, outside
        };

        // check whether the rectangle is inside or colliding
        // TODO, implement with areas
        overlap is_inside(const rect&) const;

        // parent node in the tree
        branch * m_parent;

        // bounding box
        rect m_box;
    };

    // leaf of the AABB tree
    struct leaf : public node {

        leaf(const flat::bounded& object, node* parent = nullptr)
            : node(object.bound_rect(), parent), m_object(object) {}

        // check whether it is a leaf or a branch
        virtual bool is_leaf() const override {
             return true;
        }
        
        // contained object
        const flat::bounded& m_object;
    };

    // branch of the AABB tree
    struct branch : public std::pair<node*, node*>, public node {

        friend class tree;

        // not able to construct a branch externally
        branch() = delete; 

        // check whether it is a leaf or a branch
        virtual bool is_leaf() const override {
             return false;
        }

    private:

        // NB: first and second MUST be non null
        branch(node* first, node* second, node* parent = nullptr);
            : std::pair<node*, node*>(first, second), 
              node(rect_union(first->m_box, second->m_box), parent) 
        {
            // set this object as parent
            first->m_parent = this;
            second->m_parent = this;
        }
    };


    // The tree structure
    // Spatial complexity: O(N log(N)), N = total number of leaves
    class tree {
    public: 
        // initialize an empty tree
        tree();

        // initialize a tree and emplace the first node
        tree(const flat::bounded&);

        // initialize a tree and emplace a set of objects using the "divide and conquer" algorithm
        // O(N * log(N)), N = init.size(): the list is supposed to be sorted by rectangle positioning
        // TODO, not working. A proper data structure not found yet
        template <class RandomIt> // random access iterator
        tree(RandomIt begin, RandomIt end);

        void insert(const flat::bounded&);
        void remove(const flat::bounded&);

        // find but don't allow to delete the pointed object
        // O(log(N)) time complexity, N = total number of leaves
        const leaf& find(const flat::bounded&) const;

    private:

        struct collision {
            // std::pair doesn't support referencies :/

            collision(const leaf& _first, const leaf& _second);

            const flat::bounded& first;
            const flat::bounded& second;
            node * common_parent_node;

            // checking 
            bool operator==(const collision& other) const;
        };

        leaf * find_leaf(const flat::bounded&) const;

        // find the best fitting leaf start by a node
        leaf * find_best_fit(const flat::bounded&, node*) const;

        // starting point of the tree
        // it's safe to use C pointers because it's an internal mechanism
        // and there's no reason to make our life more difficult
        node * m_root;

        // collision queue, collect current collisions states
        std::vector<collision> m_collision_queue;
        
        // recursive support for the "divide and conquer" initialization
        template <class RandomIt>
        std::shared_ptr<node> construct(RandomIt begin, RandomIt end);
    };

    template <class RandomIt> // random access iterator
    tree::tree(RandomIt begin, RandomIt end) : tree() {

        auto N = std::distance(begin, end);

        switch (N) {
        case 0:
            return;
        case 1:
            m_top = new leaf(*it, nullptr));
            return;
        default:
            break;
        }

        auto it = begin + N/2;

        m_top = std::static_pointer_cast<node>(
                std::make_unique<branch>(construct(begin, it), construct(it, end), nullptr) );
    }

    template <class RandomIt>
    node * tree::construct(RandomIt begin, RandomIt end)
    {
        auto N = std::distance(begin, end);

        if (N == 1)
            return std::static_pointer_cast<node>(std::make_shared<leaf>(*begin));

        auto it = begin + N/2;
        return std::static_pointer_cast<node>(std::make_shared<branch>(construct(begin, it), construct(it, end)));
    }
}


