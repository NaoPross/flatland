#include "window.hpp"

#include "core/signal.hpp"
#include "flatland.hpp"

#include <SDL2/SDL.h>
#include <iostream>


using namespace flat;


window::window(const std::string& title, std::size_t width, std::size_t height)
    : wsdl2::window(title, width, height) {}


void window::render()
{
    get_renderer().clear();

    for(auto&& child : *this) {
        if (child->is_visible())
            child->render();
    }

    get_renderer().present();
}



