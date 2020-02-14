#pragma once

#include <mm/mmvec.hpp>

namespace flat::geom {

    template <typename T>
    using corner = mm::vec2<T>;

    // define comparison operator overloading strictly in this namespace
    // WARNING: a < b doesn't mean !(b < a)
    inline bool operator<(const corner& a, const corner& b) {
        return a[0] < b[0] && a[1] < b[1];
    }

    inline bool operator>(const corner& a, const corner& b) {
        return a[0] > b[0] && a[1] > b[1];
    }

    /*
    * Rectangle struct n
    */
    template <typename T>
    struct rect {
        rect(const corner& _upper, const corner& _lower) : upper(_upper), lower(_lower) {}
        constexpr rect(corner _upper, corner _lower) : upper(_upper), lower(_lower) {}

        corner upper, lower; 

        double perimeter() const {
            auto d = r.upper - r.lower;
            return 2 * (d[0] + d[1]);
        }

        inline bool inscribed_in(const rect<T>& ext) const
        {
            return ext.lower < lower && upper < ext.upper;
        }

        inline bool inscribes(const rect<T>& ext) const {
            return ext.lower > lower && upper > ext.upper;
        }

        inline bool inside(const corner& c) const {
            return lower < c && upper > c;
        }

        // check for overlaps
        bool overlaps(const rect<T>& ext) const;
    };

    // construct a rectangle which embeds both rectangles
    template <typename T>
    rect<T> rect_union(const rect<T>& a, const rect<T>& b) {
        return rect({std::min(a.lower[0], b.lower[0]), std::min(a.lower[1], b.lower[1])}, 
                {std::max(a.lower[0], b.lower[0]), std::max(a.lower[1], b.lower[1])});
    }

    /*
    * TODO, Optimize the double case into the graphic card with matrices
    */
    template <typename T>
    inline bool rect<T>::overlaps(const rect<T>& ext) const
    {
        return (inside(ext.lower) && !inside(ext.upper)) ||
               (inside(ext.upper) && !inside(ext.lower));
    }
}
