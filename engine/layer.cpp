#include "layer.hpp"
#include "sprite.hpp"

using namespace flat;

const uint32_t layer::bg_level = 0;

layer::layer(  uint32_t overlap, 
        sprite * bg, 
        const std::string& id)

    : renderbase(overlap, id)
{
    set_background(bg);
}


void layer::set_background(sprite *bg)
{
    if (bg != NULL) {

        bg->set_overlap(bg_level);
        attach(bg);

    } else {
        npdebug("Cannot pass a null sprite as background")
    }
}

sprite * layer::background()
{
    sprite * out = 0;
    auto it = begin();

    while(it != end() && (*it).overlap() == bg_level)
        out = dynamic_cast<sprite*>(&(*(it++)));

    return out;
}

void layer::clear_backgrounds()
{
    // TODO, remove objects with overlap 0
}

void layer::render()
{
    for (auto& child : *this)
        child.render();
}

