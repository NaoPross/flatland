#pragma once

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include "object.hpp"
#include "task.hpp"
#include "types.h"

namespace flat
{
    namespace core
    {
        class signal;
        class channel;
    }
}

class flat::core::signal : virtual public flat::core::object
{

public:

    flat::core::object * sender;
    void * data;
    priority prior;

    signal( flat::core::object * sender, 
                const std::string& id = "", 
                void *data = 0, 
                priority prior = 5);

    /* Alias to flat::core::channel::emit() */
    bool emit(const std::string& channel) const;

    enum class priority : uint8_t
    {
        instant = 0, 
        highest, 
        high,
        none, 
        low, 
        lowest
    };
};


/* Channel class */
class flat::core::channel : virtual public flat::core::object
{
    /* Post processing signal stacking */
    stack_t stack;

    /* Listeners list */
    std::list<flat::core::listener_s*> listeners;

    /* Synchronous task checking for signals */
    task_s * checker;

    /* Channel mapping */
    static std::map<std::string, channel*> channels;    
 
public:

    flat::core::channel(const std::string& id = "", flat::core::task::priority prior = flat::core::task::priority::none);
    ~flat::core::channel();

    void emit(const flat::core::signal&);

    void connect(flat::core::listener_s*);
    void disconnect(flat::core::listener_s*);
   
    static flat::core::channel * find_channel(const std::string&); 

    void post_processing(void*);

    /* Functor for order comparison */
    struct sig_prior_cmp {
    
        bool operator()(const flat::core::signal&, const flat::core::signal&) const;
    };

    typedef std::set<flat::core::signal, sig_prior_cmp> stack_t;
};

/* Listener class */
class flat::core::listener_s : virtual public flat::core::object
{
    std::list<std::string> filters;

    bool check_in_filters(const std::string&) const;

public:

    flat::core::listener(const std::initializer_list<std::string>& filters = {});
    virtual ~flat::core::listener() {}

    virtual void execute(const flat::core::signal&) = 0;

    void add_filter(const std::string&);
    void remove_filter(const std::string&);

    bool connect(const std::string&);
    bool disconnect(const std::string&);
};

template<class T>
class flat::core::listener : public flat::core::listener_s
{
    typedef void (T::*callback_t)(void*);

    T * object;
    callback_t callback;

    void *args;

public:

    listener(   T *object,
                callback_t callback,
                const std::initializer_list<std::string>& filters = {},
                void *args = 0)

        :   flat::core::listener_s(filters), 
            object(object), 
            callback(callback),
            args(args) {}

    virtual void execute(const flat::core::signal&) override
    {
        if ((!sig.get_id().empty() && check_in_filters(sig.get_id())) || 
        (sig.get_id().empty() && filters.empty()))
        (object->*callback)(sig.sender, sig.data);
    }
};


