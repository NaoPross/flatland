#ifndef __FLATACTOR_H__
#define __FLATACTOR_H__

#include "collector.hpp"

class FlatBound;

class FlatActor : public FlatCollector
{
    // TODO, event binding
    // TODO, serial binding
    
    /* Bounds */
    FlatBound * bounds;

public:

    FlatActor(FlatCollector *parent = 0, FlatBound *bounds = 0);

    ~FlatActor();


    void setBounds(FlatBound * bounds);
    FlatBound * getBounds() const;
};

#endif
