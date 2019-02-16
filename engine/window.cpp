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
{
    rendering_task = main_job().delegate_task(&window::render, this, core::priority_t::min);
}


void window::render()
{
    for(auto& child : *this)
    {
        if (child.is_visible())
            child.render();
    }
}



