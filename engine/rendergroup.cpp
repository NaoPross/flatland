#include "rendergroup.hpp"

using namespace flat;

void rendergroup::render()
{
    for (auto& element : *this)
    {
        if (element.is_visible())
            element.render();
    }
}

