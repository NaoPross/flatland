#include "flatland/window.hpp"


using namespace flat;

window::window(const std::string& title, std::size_t width, std::size_t height)
    : wsdl2::window(title, width, height) {}


void window::render() const
{
    get_renderer().clear();

    for(auto&& child : *this) {
        if (child->visible)
            child->render();
    }

    get_renderer().present();
}



