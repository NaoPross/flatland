#ifndef __FLAT_TASK_H__
#define __FLAT_TASK_H__

#include <set>
#include "types.hpp"

struct task_prior
{
    bool operator()(task_s* s, task_s* g) const;
};

typedef std::set<task_s*, task_prior> task_set;

class task_s
{
    /* Call priority of a task */
    /* 0 = maximum priority */
    /* 5 = default priority */
    Uint8 priority;
    bool pre_process;

    /* Callback list */
    static task_set pre_process_tasks;
    static task_set post_process_tasks;

public:

    task_s(bool pre_process = false, Uint8 priority = 5);

    virtual ~task_s();

    virtual void exec() = 0;

    Uint8 getPriority() const;
    
    void setPriority(Uint8 priority);

    /* Execute tasks */
    static void executePreProcess();
    static void executePostProcess();
};

template<class T, typename arg_t = void*>
class FlatTask : public task_s
{
    typedef void (T::*callback_t)(arg_t);

    T * object;
    callback_t callback;

    arg_t args;

public:

    FlatTask(   T *object, 
                callback_t callback, 
                arg_t args, 
                bool pre_process = false, 
                Uint8 priority = 5)

        : task_s(pre_process, priority), object(object), callback(callback), args(args) {}

    virtual void exec() override
    {
        (object->*callback)(args);
    }
};

#endif
