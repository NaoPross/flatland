#include "window.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include "layer.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace flat;

window::window( const std::string& title, 
        std::size_t width, 
        std::size_t height)

    : wsdl2::window(title, width, height)
{}


void window::render()
{
    npdebug("Clearing window content")

    get_renderer().clear();

    npdebug("Rendering all content")

    for(auto& child : *this)
    {
        if (child.is_visible())
            child.render();
    }

    get_renderer().present();
}



