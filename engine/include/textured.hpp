#pragma once

#ifndef __FLAT_TEXTURED_HPP__
#define __FLAT_TEXTURED_HPP__

#include "renderbase.hpp"
#include "mm/mmvec.hpp"
#include "wsdl2/video.hpp"

namespace flat {

/*
 * Basic abstract class for texture handling,
 *
 * textures' usage divides itself into three branches:
 *
 *  - sprites <--> static
 *  - streamers <--> streaming
 *  - layers <--> target
 */
class textured : virtual public renderbase
{
    // location and rectangular size, in pixel
    wsdl2::rect m_bounds;

    // source viewport 
    wsdl2::rect m_viewport;


protected:

    virtual wsdl2::texture& texture() = 0;

public:

    textured( const wsdl2::rect& bounds,
              const wsdl2::rect& viewport, 
              const std::string& lab = "",
              uint32_t overlap = 1)
        : renderbase(overlap, lab), 
          m_bounds(bounds), 
          m_viewport(viewport) {}

    virtual ~textured() {}

    // bounds accessors
    inline void set_location(int x, int y)
    {
        m_bounds.x = x;
        m_bounds.y = y;
    }

    inline void set_location(const mm::vec2<int>& l)
    {
        m_bounds.x = l[0];
        m_bounds.y = l[1]; 
    }

    inline void set_width(std::size_t width)
    {
        m_bounds.w = static_cast<int>(width);
    }

    inline void set_height(std::size_t height)
    {
        m_bounds.h = static_cast<int>(height);
    }

    inline void set_size(std::size_t width, std::size_t height)
    {
        m_bounds.w = static_cast<int>(width);
        m_bounds.h = static_cast<int>(height);
    }

    inline void set_bounds(const wsdl2::rect& b)
    {
        m_bounds = b;
    }

    inline mm::vec2<int> location() const
    {
        return mm::vec2<int>({m_bounds.x, m_bounds.y});
    }

    inline std::size_t width() const
    {
        return m_bounds.w;
    }

    inline std::size_t height() const
    {
        return m_bounds.h;
    }
    
    inline wsdl2::rect bounds()
    {
        return m_bounds;
    }

    inline const wsdl2::rect& bounds() const
    {
        return m_bounds;
    }

    // viewport accessors
    inline void set_viewport(const wsdl2::rect& v)
    {
        m_viewport = v;
    }

    inline wsdl2::rect viewport()
    {
        return m_viewport;
    }

    inline const wsdl2::rect& viewport() const
    {
        return m_viewport;
    }

    // render the owned texture
    virtual void render() override
    {
        // TODO, generalize with flip angle
        texture().render(m_viewport, m_bounds); 
    }
};

}

#endif
