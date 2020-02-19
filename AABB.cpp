#include "flatland/AABB.hpp"

#include <algorithm>

using namespace flat::AABB;

// tree class
tree::tree() : m_root(nullptr) {

}

tree::tree(const flat::trait::bounded& object) 
    : m_root(new leaf(object)) 
{

}

void tree::insert(const flat::trait::bounded& object) {

    // check for an empty tree
    if (m_root == nullptr) {
        m_root = new leaf(object, nullptr);
        return;
    }
    
    // check for collisions
    std::stack<leaf*> collision_stack;
    collisions_check(object, collision_stack, m_root);

    // find the best fitting leaf, as fast as possible
    leaf* best = find_best_fit(object, collision_stack);

    // create the new leaf
    leaf* new_leaf = new leaf(object);
    
    branch * parent = best->m_parent;

    if (parent == nullptr) { // the root is a leaf
        m_root = new branch(parent, new_leaf, nullptr);
        return;
    }

    // check wether the best fit is in first or in second position
    // (A branch always contains two elements)
    bool is_first = parent->first == best;
    node * other = is_first ? parent->second : parent->first;

    // allocate the new branch as child of "parent"
    branch* new_branch = new branch(new_leaf, best, parent);
   
    // adjust the parent ownership 
    if (is_first)
        parent->first = new_branch;
    else
        parent->second = new_branch;
    
    // adjust the ancestors' boxes size
    refit(new_branch);

    // push the found collisions into the queue
    for(; !collision_stack.empty(); collision_stack.pop())
        m_collision_queue.insert(collision(new_leaf, collision_stack.top())); 
}

void tree::remove(const flat::trait::bounded& object) {
    
    leaf * ptr = find_leaf(object);

    if (ptr == nullptr)
        return;

    // reset ancestor
    branch * parent = ptr->m_parent;

    if (parent == nullptr) { // ptr is the root, then empty the tree
        delete m_root;
        m_root = nullptr;
    } 

    // attach the other node to the grandpa
    node * other = (ptr == parent->first) ? parent->first : parent->second;
    branch * grandpa = parent->m_parent;

    // check whether the "parent" is the root
    if (grandpa == nullptr) {
        // "other" becomes the root
        delete m_root;
        m_root = other;
    }
   
    // adjust grandpa ownership 
    if (parent == grandpa->first)
        grandpa->first = other;
    else 
        grandpa->second = other;

    // deallocate the parent, which is no more referenced
    delete parent;

    // adjust grandpa and ancestors box
    refit(grandpa);

    // remove all collisions matching "object"
    // TODO, unfortunately erase_if doesn't exists (eheh waiting for C++20)
    
}

leaf * tree::find_leaf(const flat::trait::bounded& object) const {

    if (m_root == nullptr) // empty tree
        return nullptr;
    
    rect box = object.bound_rect();
    node* ptr = m_root;

    bool legit = true;
    while(legit && !ptr->is_leaf()) {
        // look for a fully fitting rectangle
        branch * b = dynamic_cast<branch*>(ptr); // this is never nullptr

        if (is_rect_inscribed(box, b->first.m_box)) // first
            ptr = b->first;
        else if (is_rect_inscribed(box, b->second.m_box)) // second
            ptr = b->second;
        else // the object isn't surely contained in the tree
            legit = false;
    }

    leaf * out = dynamic_cast<leaf*>(ptr);

    // return the output leaf only if the two objects correspond
    return (legit && out != nullptr && &out->object == &object) ? out : nullptr;
}


void tree::refit(branch *target) {
    target.m_box = flat::geom::rect_union(first->m_box, second->m_box);

    if (target->parent != nullptr) // if it's not the root, then recurse
        refit(target->parent);
}

void tree::collisions_check(const flat::trait::bounded& box, std::stack<leaf*>& coll_stack, node *start) {
   
    // the tree is not empty and the box isn't disjoint with respect to the node box
    if (start != nullptr && !start->is_disjoint(box.aabb()))
    {
        // TODO, dynamic_cast is the simplest solution at the moment
        // TODO, use variants and std::visit instead
        if (branch * b = dynamic_cast<branch*>(start)) {
            // recurse into the tree
            collisions_check(box, coll_stack, b->first);    
            collisions_check(box, coll_stack, b->second);    
        } else if (leaf * l = dynamic_cast<leaf*>(start) && start->m_box.overlaps(box.aabb())) {
            // finally push into the stack
            coll_stack.push(l); 
        }
    }
}

const leaf& tree::find(const flat::trait::bounded& object) const {
    leaf * ptr = find_leaf(object);
   
    // not found 
    if (ptr == nullptr)
        ; // TODO, throw an exception

    return *ptr;
}

leaf * tree::find_best_fit(const flat::trait::bounded& obj, node * current, std::stack<leaf*>& collision_stack) const {
    
    // TODO, dynamic_cast is the simplest solution at the moment
    // TODO, use variants and std::visit instead
    // if it's a leaf, then return it
    if (current->is_leaf()) 
        return dynamic_cast<leaf*>(current);

    // evaluate and minimize generated perimeter by the union of the object and the node box
    branch * b = dynamic_cast<branch*>(current);    
    double p1 = perimeter(rect_union(b->first->m_box, obj.enclosing_box()));
    double p2 = perimeter(rect_union(b->second->m_box, obj.enclosing_box()));

    // recurse onto the next branch
    return (p1 < p2) ? 
        tree::find_best_fit(obj, b->first) : 
        tree::find_best_fit(obj, b->second);   
}
