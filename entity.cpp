#include "flatland/entity.hpp"

using namespace flat;

entity::entity(vector_type pos)
    : m_pos(pos), m_bound(entity::bound::none) {}

entity::entity(vector_type pos, wsdl2::rect rbound)
    : m_pos(pos), m_bound(entity::bound::rectangles)
{
    m_bound_rects.insert(rbound);
}

bool entity::collides(const entity& other) const
{
    // no bound
    if (m_bound == bound::none || other.m_bound == bound::none)
        return false;

    // rectangle(s) bound
    if (other.m_bound == bound::rectangles) {
        // TODO: check if this is actually faster, and improve
        
        // check if they are in the same quadrant
        std::pair<int, int> quad = std::make_pair(
            ((0 < other.position().x()) - (other.position().x() < 0)),
            ((0 < other.position().y()) - (other.position().y() < 0))
        );

        std::pair<int, int> other_quad = std::make_pair(
            ((0 < other.position().x()) - (other.position().x() < 0)),
            ((0 < other.position().y()) - (other.position().y() < 0))
        );

        decltype(m_bound_rects)::iterator other_begin, other_end;
        if (quad == other_quad) {
            // reverse iterator
            other_begin = other.m_bound_rects.begin();
            other_end = other.m_bound_rects.end();
        } else {
            // forward iterator
            other_begin = other.m_bound_rects.begin();
            other_end = other.m_bound_rects.end();
        }

        // if they are colliding it should take less time otherwise
        // the loop will take m_bound_rects.size() * other.m_bound_rects.size()
        // iterations
        for (auto&& it = m_bound_rects.begin(); it != m_bound_rects.end(); ++it)
            for (auto&& other_it = other_begin; other_it != other_end; ++other_it)
                if (it->intersects(*other_it))
                    return true;

        return false;
    }

    throw std::logic_error("this type of bound collision not implemented");
    return false;
}

wsdl2::rect entity::enclosing_rect() const
{
    if (m_bound == bound::none) {
        return {0, 0, 0, 0};
    }

    if (m_bound == bound::rectangles) {
        if (m_bound_rects.size() == 1) {
            return *(m_bound_rects.cbegin());
        } else {
            return std::accumulate(
                // skip first element
                ++(m_bound_rects.cbegin()),
                m_bound_rects.cend(),
                // because given here
                *(m_bound_rects.begin()),
                [](wsdl2::rect prev, const wsdl2::rect& next) -> wsdl2::rect {
                    return prev.union_with(next);
                }
            );
                
        }
    }
}
