#pragma once

#include "mm/mmvec.hpp"

#include <variant>
#include <utility>

namespace flat
{
    class bounded
    {
    public:
        using vector_type = mm::vec2<int>;

        enum class shape
        {
            rectangle,
            circle,
            from_points,
        };

        bounded() = delete;
        virtual ~bounded() {}

        bounded(unsigned radius);
        bounded(unsigned width, unsigned height);

        // TODO: think how to do this
        // template<template<typename> typename Container>
        // bounded(Container<vector_type>&& points) {}

        virtual const vector_type& position() const = 0;

        shape bound_shape() { return m_shape; }

        bool collides(const bounded& other);

    private:
        bool m_can_collide;
        shape m_shape;
        std::variant<unsigned, std::pair<unsigned, unsigned>> m_size;
    };


    class entity : public bounded
    {
    public:
        using vector_type = mm::vec2<int>;

        virtual inline void move(vector_type&& delta) { m_pos += delta; }
        virtual inline void position(vector_type&& pos) { m_pos = pos; }
        virtual inline const vector_type& position() const override
        {
            return m_pos;
        }


    private:
        vector_type m_pos;
    };

    
    class actor : public entity
    {
    public:
        const unsigned max_hp = 100;

    private:
        unsigned m_hp;
    };
}
