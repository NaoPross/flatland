#ifndef __FLATLAYER_H__
#define __FLATLAYER_H__

#include "renderbase.hpp"

namespace flat {

class sprite;

struct layer : virtual public renderbase, public renderbase::collector
{
    layer(uint32_t overlap = 1, sprite * bg = 0);

    // background modifiers
    void set_background(sprite * bg);
    sprite * background();

    void clear_backgrounds();

    static const uint32_t bg_level;

    // render function
    virtual void render() override;
};

}

#endif
