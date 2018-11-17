#include "flatactor.h"

#include "flattask.h"
#include "flatland.h"

FlatActor::FlatActor(FlatCollector *parent, bool evolving)

    : FlatCollector(parent), task(0)
{
    setEvolving(evolving);
}

FlatActor::~FlatActor()
{
    if (task)
        delete task;
}

void FlatActor::evolve_task(void *data)
{
    evolve(*(float*)data);
}

void FlatActor::evolve(float) {}

void FlatActor::setEvolving(bool flag)
{
    if (isEvolving() != flag)
    {
        if (flag)
            task = new FlatTask<FlatActor>(this, &FlatActor::evolve_task, (void*)&flatland_dt);
        else {

            delete task;
            task = 0;
        }
    }
}

bool FlatActor::isEvolving() const
{
    return task != 0;
}

