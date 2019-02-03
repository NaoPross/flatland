#ifndef __FLATLAYER_H__
#define __FLATLAYER_H__

#include "collector.hpp"

namespace flat {

struct layer : public renderbase, public core::child<renderbase>, public core::collector<renderbase>
{
    using core::child<renderbase>::child;    

    virtual void render() override;

    // TODO add features
};

}

#endif
