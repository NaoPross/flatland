#ifndef __FLATACTOR_H__
#define __FLATACTOR_H__

#include "flatcollector.h"
#include "flatevolvable.h"

class FlatBound;

class FlatActor : public FlatCollector, public FlatEvolvable
{
    // TODO, event binding
    // TODO, serial binding
    
    /* Bounds */
    FlatBound * bounds;

public:

    FlatActor(FlatCollector *parent = 0, FlatBound *bounds = 0);

    ~FlatActor();

    virtual void evolve(float dt) override;

    void setBounds(FlatBound * bounds);
    FlatBound * getBounds() const;
};

#endif
