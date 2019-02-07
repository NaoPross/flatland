#ifndef __FLATACTOR_H__
#define __FLATACTOR_H__

#include "renderbase.hpp"

namespace flat {

struct actor : public renderbase, public renderbase::child
{
    // TODO, serial binding
    
    /* Bounds */
    //bounds * bounds;

public:

    //actor(actor *parent = 0, FlatBound *bounds = 0);
    
    using renderbase::renderbase;

    // TODO add features

    //~actor();

    //void setBounds(FlatBound * bounds);
    //FlatBound * getBounds() const;
};

}

#endif
