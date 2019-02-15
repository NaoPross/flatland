#pragma once

#ifndef __EVOLVABLE_H__
#define __EVOLVABLE_H__

#include "task.hpp"

namespace flat::core {

/*
 * A confort util in case of need
 */
class evolvable
{
    // task for the evolution of the system
    std::shared_ptr<core::task> evolving_task;

protected:

    /*
     * Pure virtual call which evolves the described system
     *
     * This call is performed by a "none" priority task, 
     * then it's surely called before "render"
     */
    virtual void evolve() = 0;

public:

    evolvable()
    {
        evolving_task = main_job().delegate_task(&animation::evolve, this);
    }

    inline void resume()
    {
        if (!is_evolving())
            evolving_task = main_job().delegate_task(&animation::evolve, this);
    }

    inline void pause()
    {
        if (is_evolving())
            evolving_task = nullptr; // this should remove the task
    }

    inline bool is_evolving() const
    {
        return evolving_task != nullptr;
    }

    // this method must be public
    void evolution_call()
    {
        evolve();
    }
};

}

#endif
