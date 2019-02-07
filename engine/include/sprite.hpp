#ifndef __FLATSPRITE_H__
#define __FLATSPRITE_H__

#include "renderbase.hpp"

namespace flat {

struct sprite : virtual public renderbase
{
    using renderbase::renderbase;

    virtual void render() override;

    // TODO add features
};

}

#endif
