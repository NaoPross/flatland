#ifndef __FLAT_SIGNAL_H__
#define __FLAT_SIGNAL_H__

#include <vector>
#include <map>
#include "flatobject.h"

class task_s;

struct FlatSignal : virtual public FlatObject
{
    const int type;
    void * data;

    FlatSignal(int type, void *data);

    static emit(const FlatSignal&);
    static std::vector<FlatSignal> sig_stack;
};

template <class T>
class FlatListener : virtual public FlatObject
{
    typedef void (T::*callback_t)(void*);

    task_s * listener;

    void checkSignal(void*)
    {
        for (auto signal : FlatSignal::sig_stack)
        {
            callback_t cb;

            if (cb = checkmap.find(signal.type))
                cb(signal.data);
        }
    }

    std::map<int, callback_t> checkmap;

public:

    FlatListener()
    {

    }

    ~FlatListener()
    {

    }

    connect(int type, callback_t cb)
    {
        checkmap.insert(std::make_pair(type, cb));
    }
};


#endif
