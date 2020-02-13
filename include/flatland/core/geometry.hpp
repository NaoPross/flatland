#pragma once

#include <mm/mmvec.hpp>

namespace flat::geom {
    /*
    * Rectangle struct n
    */
    template <typename T>
    struct rect {
        rect(const mm::vec2<T>& _upper, const mm::vec2<T>& _lower) : upper(_upper), lower(_lower) {}
        constexpr rect(mm::vec2<T> _upper, mm::vec2<T> _lower) : upper(_upper), lower(_lower) {}

        mm::vec2<T> upper, lower; 

        double perimeter() const {
            auto d = r.upper - r.lower;
            return 2 * (d[0] + d[1]);
        }

        bool inscribed_in(const rect<T>& ext) const
        {
            return ext.lower[0] < lower[0] &&
                ext.lower[1] < lower[1] &&
                upper[0] < ext.upper[0] &&
                upper[1] < ext.upper[1];
        }
    };

    // construct a rectangle which embeds both rectangles
    template <typename T>
    rect<T> rect_union(const rect<T>& a, const rect<T>& b) {
        return rect({std::min(a.lower[0], b.lower[0]), std::min(a.lower[1], b.lower[1])}, 
                {std::max(a.lower[0], b.lower[0]), std::max(a.lower[1], b.lower[1])});
    }
}
