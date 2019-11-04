#include "AABB.hpp"

#include <algorithm>

using namespace flat::AABB;

// perimeter, abstract surface bi-dimensional measure
double perimeter(const rect& r) const {
    auto d = r.upper - r.lower;
    return 2 * (d[0] + d[1]);
}

// construct a rectangle to embed both rectangles
rect rect_union(const rect& a, const rect& b) const {
    return rect({std::min(a.lower[0], b.lower[0]), std::min(a.lower[1], b.lower[1])}, 
                {std::max(a.lower[0], b.lower[0]), std::max(a.lower[1], b.lower[1])});
}

// tree class
tree::tree() : m_top(nullptr) {

}

tree::tree(const flat::bounded& object) 
    : m_top(std::static_pointer_cast<node>(std::make_unique<leaf>(object, nullptr))) 
{

}

void insert(const flat::bounded& object) {

}

void remove(const flat::bounded& object) {

}
