#pragma once

#include <mm/mmvec.hpp>

#include <utility>

namespace flat
{
    namespace trait
    {
        struct positioned
        {
            virtual ~positioned() = default;

            // getter
            virtual mm::vec2<int>& pos() = 0;
            virtual const mm::vec2<int>& pos() const { return pos(); }

            // setter
            virtual void pos(const mm::vec2<int>& newpos) = 0;
            virtual void pos(mm::vec2<int>&& newpos)
            {
                pos(std::forward<mm::vec2<int>>(newpos));
            }

            virtual void move(mm::vec2<int> v) { pos(pos() + v); }
        };
    }
}
