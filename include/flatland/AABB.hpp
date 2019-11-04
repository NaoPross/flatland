#pragma once

#include "flatland/trait/bounded.hpp"

// Axys aligned bounding box interface
namespace flat::AABB {
    
    // tools for a flat::rect treating
    
    using rect = flat::rect<double>
    
    // perimeter, abstract surface bi-dimensional measure
    double perimeter(const rect&) const;

    // construct a rectangle to embed both rectangles
    rect rect_union(const rect&, const rect&) const;

    // node of the tree
    struct node {

        node(const rect& box, std::shared_ptr<node> parent = nullptr) 
            : m_box(box), m_parent(parent) {}
        
        virtual ~node();
        
        // rectangle bounds, lower = bottom-left corner, upper = top-right corner
        inline const mm::vec2<double>& lower_bound() const {
            return m_box.lower;
        }
        inline const mm::vec2<double>& upper_bound() const {
            return m_box.upper;
        }

        // check whether it is a leaf or a branch
        virtual bool is_leaf() const = 0;
        
        // check if a node is inside
        // bool is_inside(const node&) const;

        // parent node in the tree
        shared_ptr<node> m_parent;

        // bounding box
        rect m_box;
    };

    // leaf of the AABB tree
    struct leaf : public node {

        leaf(const flat::bounded& object, std::shared_ptr<node> parent = nullptr)
            : node(object.bound_rect(), parent), m_object(object) {}

        // check whether it is a leaf or a branch
        virtual bool is_leaf() const override {
             return true;
        }
        
        // contained object
        const flat::bounded& m_object;
    };

    // branch of the AABB tree
    struct branch : public std::pair<std::shared_ptr<node>, std::shared_ptr<node>>, public node {

        branch(std::shared_ptr<node> first, std::shared_ptr<node> second, std::shared_ptr<node> parent = nullptr);
            : std::pair<std::shared_ptr<node>, std::shared_ptr<node>>(first, second), 
              node(rect_union(first->m_box, second->m_box), parent) 
        {
            first->m_parent = this;
            second->m_parent = this;
        }

        // check whether it is a leaf or a branch
        virtual bool is_leaf() const override {
             return false;
        }
    };


    // The tree structure
    class tree {
    public: 
        // initialize an empty tree
        tree();

        // initialize a tree and emplace the first node
        tree(const flat::bounded&);

        // initialize a tree and emplace a set of objects using the "divide and conquer" algorithm
        // O(N * log(N)), N = init.size(): the list is supposed to be sorted by rectangle positioning
        template <class RandomIt> // random access iterator
        tree(RandomIt begin, RandomIt end);

        void insert(const flat::bounded&);
        void remove(const flat::bounded&);

    private:
        // find the best fitting node
        std::shared_ptr<node> best_fit(flat::bounded&) const;

        // starting point of the tree
        std::unique_ptr<node> m_top;

        // collision queue, collect new collisions
        std::vector<std::pair<std::shared_ptr<flat::bounded>, std::shared_ptr<flat::bounded>> m_collision_queue;
        
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
            m_top = std::static_pointer_cast<node>(std::make_unique<leaf>(*it, nullptr));
            return;
        default:
            break;
        }

        auto it = begin + N/2;

        m_top = std::static_pointer_cast<node>(
                std::make_unique<branch>(construct(begin, it), construct(it, end), nullptr) );
    }

    template <class RandomIt>
    std::shared_ptr<node> tree::construct(RandomIt begin, RandomIt end)
    {
        auto N = std::distance(begin, end);

        if (N == 1)
            return std::static_pointer_cast<node>(std::make_shared<leaf>(*begin));

        auto it = begin + N/2;
        return std::static_pointer_cast<node>(std::make_shared<branch>(construct(begin, it), construct(it, end)));
    }
}


