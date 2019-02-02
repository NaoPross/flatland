#ifndef __FLATACTOR_H__
#define __FLATACTOR_H__

#include "component.hpp"

namespace flat {

class bounds;

class actor : public 
{
    // TODO, event binding
    // TODO, serial binding
    
    /* Bounds */
    bounds * bounds;

public:

    FlatActor(F *parent = 0, FlatBound *bounds = 0);

    ~FlatActor();


    void setBounds(FlatBound * bounds);
    FlatBound * getBounds() const;
};

}

#endif
