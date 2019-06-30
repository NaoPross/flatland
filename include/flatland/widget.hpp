#pragma once

#include "sprite.hpp"
#include "trait/renderable.hpp"

namespace flat
{
    class widget : public sprite, public rendergroup
    {
    public:
        virtual void render() const override;
    };
}
