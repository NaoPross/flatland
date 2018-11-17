#ifndef __FLATACTOR_H__
#define __FLATACTOR_H__

#include "flatcollector.h"

struct task_s;

class FlatActor : public FlatCollector
{
    task_s * task;
    
    // TODO, event binding
    // TODO, serial binding

public:

    FlatActor(FlatCollector *parent = 0, bool evolving = false);

    virtual ~FlatActor();

    /* Evolution fields */

    void evolve_task(void*);

    virtual void evolve(float dt);

    void setEvolving(bool flag);

    bool isEvolving() const;
    
    /* Surface fields */

    virtual SDL_Surface * sdl_surface() = 0;
};

#endif
