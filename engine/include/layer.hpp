#ifndef __FLATLAYER_H__
#define __FLATLAYER_H__

#include "renderbase.hpp"

namespace flat {

struct layer : virtual public renderbase, public renderbase::child, public renderbase::collector
{
    using renderbase::renderbase;    

    virtual void render() override;

    // TODO add features
};

}

#endif
