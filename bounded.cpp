#include "flatland/trait/bounded.hpp"

using namespace flat;

/* namespace bound */

/// collision between types
template<>
bool bound::are_overlapping<>(const rectangle& r, const rectangles& rs)
{
    for (auto&& i : rs.m_rects)
        if (r.m_rect.intersects(i))
            return true;

    return false;
}

template<>
bool bound::are_overlapping<>(const bound::rectangle& , const bound::circle& )
{
    std::logic_error("not implemented");
    return false;
}

template<>
bool bound::are_overlapping<>(const bound::rectangles& , const bound::circle& )
{
    std::logic_error("not implemented");
    return false;
}

/* bound::rectangle */

bool bound::rectangle::overlaps(const bound::rectangle& other) const
{
    return other.m_rect.intersects(m_rect);
}



/* bound::circle */

bool bound::circle::overlaps(const bound::circle& other) const
{
    if (static_cast<unsigned>((m_pos - other.m_pos).length()) <= (m_radius + other.m_radius))
        return true;

    return false;
}

wsdl2::rect bound::circle::enclosing_rect() const
{
    return {
        static_cast<int>(m_pos.x() - m_radius),
        static_cast<int>(m_pos.y() - m_radius),
        static_cast<int>(m_radius),
        static_cast<int>(m_radius)
    };
}


/* bound::rectangles */

// TODO: check if this is actually faster, and improve
bool bound::rectangles::overlaps(const bound::rectangles& other) const
{
    // check if they are in the same quadrant
    std::pair<int, int> quad = std::make_pair(
        ((0 < other.m_rects.begin()->x) - (other.m_rects.begin()->x < 0)),
        ((0 < other.m_rects.begin()->y) - (other.m_rects.begin()->y < 0))
    );

    std::pair<int, int> other_quad = std::make_pair(
        ((0 < other.m_rects.begin()->x) - (other.m_rects.begin()->x < 0)),
        ((0 < other.m_rects.begin()->y) - (other.m_rects.begin()->y < 0))
    );

    // TODO: check for width / height to skip if they are not in the
    //       same quadrant

    // closure to check for intersections
    auto check_intersections =
        [](auto begin, auto end, auto obegin, auto oend) -> bool{
        // if they are overlapping it should take less time otherwise
        // the loop will take m_rects.size() * other.m_rects.size()
        // iterations
        for (auto&& it = begin; it != end; ++it)
            for (auto&& other_it = obegin; other_it != oend; ++other_it)
                if (it->intersects(*other_it))
                    return true;

        return false;
    };

    if (quad == other_quad) {
        // reverse iterator
        return check_intersections(
            m_rects.begin(),
            m_rects.end(),
            other.m_rects.begin(),
            other.m_rects.end()
        );
    }

    // forward iterator
    return check_intersections(
        m_rects.begin(),
        m_rects.end(),
        other.m_rects.begin(),
        other.m_rects.end()
    );
}

wsdl2::rect bound::rectangles::enclosing_rect() const
{
    if (m_rects.size() == 1) {
        return *(m_rects.cbegin());
    } else {
        return std::accumulate(
            // skip first element
            ++(m_rects.cbegin()),
            m_rects.cend(),
            // because given here
            *(m_rects.begin()),
            [](wsdl2::rect prev, const wsdl2::rect& next) -> wsdl2::rect {
                return prev.union_with(next);
            }
        );

    }
}

// ebox section
/*upair_set<std::weak_ptr<bounded>> ebox::check_collisions(const_iterator down, const_iterator up, const std::size_t depth) const {

    const auto N = std::distance(down, up);

    switch (N) {
        case 0: // no element
           return {}; 
        case 1: // one element
           return {};
        case 2:
           return {std::make_pair(*down, *(--up))};
           // divide range
        default:
           break;
    }
    auto split = std::lower_bound(down, up, 
}*/
