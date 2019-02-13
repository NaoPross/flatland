#include "sprite.hpp"
#include "flatland.cpp"

using namespace flat;

/* 
 * Sprite section
 */ 

sprite( std::size_t width, 
        std::size_t height,
        wsdl2::pixelformat::format p,
        wsdl2::texture::access a)
    
    : wsdl2::texture(*renderer(), p, a, width, height)
{

}


void sprite::render()
{
    // TODO, finally render that f***cking texture
}

void sprite::focus()
{
    core_channel().emit(focus_call(this)); 
}

/*
 * Animation section
 */

animation::animation(   std::size_t width, 
                        std::size_t height,
                        wsdl2::pixelformat::format p = wsdl2::pixelformat::format::unknown)

    : sprite(width, height, p, wsdl2::texture::access::streaming) 
{
    evolving_task = main_job().delegate_task(&animation::evolve, this);
}

void animation::resume()
{
    if (!is_evolving())
        evolving_task = main_job().delegate_task(&animation::evolve, this);
}

void animation::pause()
{
    if (is_evolving())
        evolving_task = nullptr; // this should remove the task
}

