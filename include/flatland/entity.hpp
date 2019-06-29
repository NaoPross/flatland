#pragma once

#include "mm/mmvec.hpp"
#include "wsdl2/video.hpp"

#include <variant>
#include <utility>
#include <vector>
#include <set>

namespace flat
{
    class entity
    {
    public:
        enum class bound
        {
            none, rectangles
        };

        using vector_type = mm::vec2<int>;

        /// no bound
        entity(vector_type pos);
        /// rectangular bound
        entity(vector_type pos, wsdl2::rect rbound);
        /// mask bound
        // template<typename Iter>
        // entity(vector_type pos, Iter bound_begin, Iter bounds_end);

        bool collides(const entity& other) const;

        inline void move(vector_type&& delta) { m_pos += delta; }

        inline void position(vector_type&& pos) { m_pos = pos; }
        inline const vector_type& position() const { return m_pos; }

        wsdl2::rect enclosing_rect() const;

    private:
        struct closer_to_origin
        {
            // true if a is closer (0,0) than b
            bool operator()(const wsdl2::rect& a, const wsdl2::rect& b)
            {
                return (((a.x * a.x) + (a.y * a.y)) < ((b.x * b.x) + (b.y * b.y)));
            }
        };

        vector_type m_pos;
        const bound m_bound;
        std::multiset<wsdl2::rect, closer_to_origin> m_bound_rects;
    };
}
