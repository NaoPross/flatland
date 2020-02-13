#include "AABB.hpp"

#include <algorithm>

using namespace flat::AABB;

// tree::collision struct

tree::collision::collision(const leaf& _first, const leaf& _second) 
    : first(_first.object), second(_second.object), common_parent_node(nullptr)
{
    // find the common parent node
    // TODO
}

bool tree::collision::operator==(const collision& other) const {
    // TODO
}

// tree class
tree::tree() : m_root(nullptr) {

}

tree::tree(const flat::bounded& object) 
    : m_root(new leaf(object)) 
{

}

void tree::insert(const flat::bounded& object) {

    // check for an empty tree
    if (m_root == nullptr) {
        m_root = new leaf(object, nullptr);
        return;
    }

    // find the best fitting leaf, as fast as possible
    leaf* best = find_best_fit(object);

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

    // resize the ancestors boxes in order to fit in
    // O(log(N)), N = total number of leaves
    branch * ptr = new_branch;
    while(ptr != nullptr) {
        ptr->m_box = rect_union(ptr->first.m_box, ptr->second.m_box);
        ptr = ptr->m_parent;
    }

    // TODO, check collision and emplace into the queue
}

void tree::remove(const flat::bounded& object) {
    
    leaf * ptr = find_leaf(object);

    if (ptr == nullptr)
        return;

    // reset ancestor
    branch * parent = ptr->m_parent;

    if (parent == nullptr) { // ptr is the root
        delete m_root;
        m_root = nullptr;
    } 

    // attach the other node to the grandpa
    node * other = (ptr == parent->first) ? parent->first : parent->second;
    branch * grandpa = parent->m_parent;

    // check wether the "parent" is the root
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

    // TODO, adjust grandpa and ancestors box
    // TODO, remove all collisions matching "object"
}

leaf * tree::find_leaf(const flat::bounded& object) const {

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

const leaf& tree::find(const flat::bounded& object) const {
    leaf * ptr = find_leaf(object);
   
    // not found 
    if (ptr == nullptr)
        ; // TODO, throw an exception

    return *ptr;
}

leaf * tree::find_best_fit(const flat::bounded& obj, node * current) const {

    // if it's a leaf, then return it
    if (current->is_leaf()) 
        return dynamic_cast<leaf*>(current);

    // evaluate and minimize generated perimeter by the union of the object and the node box
    branch * b = dynamic_cast<branch*>(current);    
    double p1 = perimeter(rect_union(b->first->m_box, obj.enclosing_box()));
    double p2 = perimeter(rect_union(b->second->m_box, obj.enclosing_box()));

    return (p1 < p2) ? 
        tree::find_best_fit(obj, b->first) : 
        tree::find_best_fit(obj, b->second);   
}
