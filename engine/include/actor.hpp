#ifndef __FLATACTOR_H__
#define __FLATACTOR_H__

#include "renderbase.hpp"
#include "core/collector.hpp"

namespace flat {

struct actor : public renderbase, public core::child<renderbase>
{
    // TODO, serial binding
    
    /* Bounds */
    //bounds * bounds;

public:

    //actor(actor *parent = 0, FlatBound *bounds = 0);
    
    using core::child<renderbase>::child;

    // TODO add features

    //~actor();

    //void setBounds(FlatBound * bounds);
    //FlatBound * getBounds() const;
};

}

#endif
