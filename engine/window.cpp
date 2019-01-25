#include "window.hpp"
#include "core/signal.hpp"
#include "layer.hpp"


using namespace flat;

window::window(const std::string& title, std::size_t width, std::size_t height)
    : wsdl2::window(title, width, height)
{
    
}

window::~window() {
    // nothing to destroy yet
}
