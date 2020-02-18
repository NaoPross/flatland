#pragma once

#include <mm/mmvec.hpp>

namespace flat::geom {

    template <typename T>
    using corner = mm::vec2<T>;

    // define comparison operator overloading strictly in this namespace
    // WARNING: a < b doesn't mean !(b < a)
    template <typename T>
    inline bool operator<(const corner<T>& a, const corner<T>& b) {
        return a[0] < b[0] && a[1] < b[1];
    }

    template <typename T>
    inline bool operator>(const corner<T>& a, const corner<T>& b) {
        return a[0] > b[0] && a[1] > b[1];
    }

    /*
    * Rectangle struct n
    */
    template <typename T>
    struct rect {

        rect(const corner<T>& _upper, const corner<T>& _lower) : upper(_upper), lower(_lower) {}

        rect(wsdl2::rect&& r) : 
            lower({static_cast<T>(r.x), static_cast<T>(r.y)}), 
            upper({static_cast<T>(r.x + r.w), static_cast<T>(r.y + r.h)}) {}

        corner<T> upper, lower; 

        double perimeter() const {
            auto d = upper - lower;
            return 2 * (d[0] + d[1]);
        }

        inline bool inscribed_in(const rect<T>& ext) const
        {
            return ext.lower < lower && upper < ext.upper;
        }

        inline bool inscribes(const rect<T>& ext) const {
            return ext.lower > lower && upper > ext.upper;
        }

        inline bool inside(const corner<T>& c) const {
            return lower < c && upper > c;
        }

        // lower corner x position
        inline const T& x() const {
            return lower[0];
        }

        // lower corner y position
        inline const T& y() const {
            return lower[1];
        }

        inline T width() const {
            return upper[0] - lower[0];
        }

        inline T height() const {
            return upper[1] - lower[1];
        }

        /* Check whether a rectangle is overlapping
        *  An overlap is defined as the intersection between
        *  the rectangles boundaries
        */
        bool overlaps(const rect<T>& ext) const;
    };

    // construct a rectangle which embeds both rectangles
    template <typename T>
    rect<T> rect_union(const rect<T>& a, const rect<T>& b) {
        return rect<T>({std::min(a.lower[0], b.lower[0]), std::min(a.lower[1], b.lower[1])}, 
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
