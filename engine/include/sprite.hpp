#ifndef __FLATSPRITE_H__
#define __FLATSPRITE_H__

#include "renderbase.hpp"
#include "wsdl2/video.hpp"
#include <list>

namespace flat {

/*
 *  Textured sprite, used for static rendering
 */
class sprite : virtual public renderbase : public wsdl2::texture
{

public:

    /*
     * Calling a sprite before entering flatland loop
     * funtion will surely core dump
     */
    // TODO renderbase
    sprite( std::size_t width, 
            std::size_t height,
            wsdl2::pixelformat::format p = wsdl2::pixelformat::format::unknown, 
            wsdl2::texture::access a = wsdl2::texture::access::static_);

    struct focus_call
    {
        focus_call(sprite *s) : sender(s) {}

        sprite * sender;
    };

    // send a focus signal (destinated to the owner actor)
    void focus();

    // TODO add other features
    
    // render the texture
    virtual void render() override;
};

/*
 *  Animation wrapper, used for evolving rendering
 *  Applications: light effects, waves, ecc..
 */
class animation : public sprite
{
    // task for the evolution of the system
    std::shared_ptr<core::task> evolving_task;

protected:

    /*
     * Pure virtual call which evolves the described system
     *
     * This call is performed by a "none" priority task, 
     * then it's surely called before "render"
     */
    virtual void evolve() = 0;

public:

    // access = streaming
    animation(  std::size_t width, 
                std::size_t height,
                wsdl2::pixelformat::format p = wsdl2::pixelformat::format::unknown);

    void resume(); 
    void pause();

    inline bool is_evolving() const
    {
        reutrn evolving_task != nullptr;
    }
};

}

#endif
