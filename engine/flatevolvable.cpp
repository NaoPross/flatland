#include "flatevolvable.h"
#include "flattask.h"
#include "flatland.h"

FlatEvolvable::FlatEvolvable(bool evolving)

    : task(0)
{
    setEvolving(evolving);
}

FlatEvolvable::~FlatEvolvable()
{
    if (task)
        delete task;
}

void FlatEvolvable::evolve_task(float *data)
{
    evolve(*data);
}

void FlatEvolvable::setEvolving(bool flag)
{
    if (isEvolving() != flag)
    {
        if (flag)
            task = new FlatTask<FlatEvolvable, float*>(this, &FlatEvolvable::evolve_task, &flatland_dt);
        else {

            delete task;
            task = 0;
        }
    }
}

bool FlatEvolvable::isEvolving() const
{
    return task != 0;
}

