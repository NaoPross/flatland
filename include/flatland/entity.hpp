#pragma once

#include "mm/mmvec.hpp"
#include "wsdl2/video.hpp"

#include <variant>
#include <utility>
#include <vector>

namespace flat
{
    class entity
    {
    public:
        using vector_type = mm::vec2<int>;

        enum class bound_shape {
            rectangle, circle, from_points
        };

        entity(vector_type pos, vector_type bound);
        entity(vector_type pos, unsigned radius);

        std::pair<bound_shape, std::variant<unsigned, vector_type>> bound() const;
        bool collides(const entity& other) const;

        inline void move(vector_type&& delta)
        {
            m_pos += delta;
        }

        inline void position(vector_type&& pos)
        {
            m_pos = pos;
        }

        inline const vector_type& position() const
        {
            return m_pos;
        }

        /// return a rectangle circumscribing the bound
        wsdl2::rect rect() const;

    private:
        vector_type m_pos;
        bound_shape m_bound_shape;
        // TODO: add from_points with Container<vector_type>
        std::variant<unsigned, vector_type> m_bound;
    };


    // TODO: this is a placeholder class
    class actor : public entity
    {
    public:
        const unsigned max_hp = 100;

    private:
        unsigned m_hp;
    };
}
