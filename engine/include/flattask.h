#ifndef __FLAT_TASK_H__
#define __FLAT_TASK_H__

#include <list>

class task_s
{

    /* Callback list */
    static std::list<task_s*> tasks;

public:

    task_s();

    virtual ~task_s();

    virtual void exec() = 0;

    /* Execute all tasks */
    static void executeAll();
};

template<class T>
class FlatTask : public task_s
{
    typedef void (T::*callback_t)(void*);

    T * object;
    callback_t callback;

    void * args;

public:

    FlatTask(T *object, callback_t callback, void* args)

        : task_s(), object(object), callback(callback), args(args) {}

    virtual void exec() override
    {
        (object->*callback)(args);
    }
};

#endif
