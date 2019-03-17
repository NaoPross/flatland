#pragma once

#ifndef __FLATLAYER_H__
#define __FLATLAYER_H__

#include "textured.hpp"

namespace flat {

/*
 * A class that handles grouping, but it uses a background base image
 *
 * In particular, it uses a texture in TARGET mode, in order
 * to clip elements graphically inside the background.
 *
 * Warning: at the moment, the loaded image is unique and cannot be shared to
 *          another layer instance
 */
class layer : public textured, public renderbase::collector
{
    using renderbase::collector::attach;
    using renderbase::collector::detach;

    // texture target for rendering
    wsdl2::texture m_texture;

    virtual wsdl2::texture& texture() override
    {
        return m_texture;
    }

    const wsdl2::texture& texture() const
    {
        return m_texture;
    }

    /*
     * Set m_texture without copying
     *
     * Needed for loading textures from a file
     * (such that it is an rvalue) and setting it directly
     */

    /*layer( wsdl2::texture&& tex,
           const wsdl2::rect& bounds,
           const wsdl2::rect& viewport, 
           const std::string& lab = "",
           uint32_t overlap = 1)
      : m_texture(tex), // use move constructor
        textured(bounds, viewport, lab, overlap) {}*/

public:
    
    // this should inherit the public constructor as public
    // TODO, correctly written?
    using textured::textured;

    // collector methods overloading
    inline void attach(textured* obj)
    {
        // TODO, set target
        renderbase::collector::attach(obj);
    }

    inline void detach(textured* obj)
    {
        // TODO, unset target
        renderbase::collector::detach(obj);
    }

    /*template<class S, class ...Args>
    inline S * attach(Args&&... args)
    {
        S * s = renderbase::collector::attach(args...);
        // TODO, set target
        return s;
    }*/

    //static textured * from_file(const std::string&);
};

}

#endif
