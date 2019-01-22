#ifndef __FLATEVOLVABLE_H__
#define __FLATEVOLVABLE_H__

#include "object.h"

class task_s;

class FlatEvolvable : virtual public flat::core::object
{
    task_s * task;

public:

    FlatEvolvable(bool evolving = false);

    virtual ~FlatEvolvable();

    /* Evolution fields */

    void evolve_task(float*);

    virtual void evolve(float dt) = 0;

    void setEvolving(bool flag);

    bool isEvolving() const;
};

#endif
