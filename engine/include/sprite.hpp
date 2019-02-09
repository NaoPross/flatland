#ifndef __FLATSPRITE_H__
#define __FLATSPRITE_H__

#include "renderbase.hpp"
#include "wsdl2/video.hpp"

namespace flat {

class sprite : virtual public renderbase
{
    wsdl2::texture m_texture;

    bool m_focused;

public:

    using renderbase::renderbase;

    // texture accessors
    wsdl2::texture& texture();
    const wsdl2::texture& texture() const;

    // manage focus
    void set_focused(bool);
    bool focused() const;

    // render the texture
    virtual void render() override;

    // TODO add features
};

}

#endif
