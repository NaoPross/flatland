#pragma once

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include "object.hpp"
#include "task.hpp"
#include "types.h"
#include <functional>
#include "priority.hpp"

namespace flat
{
    namespace core
    {

    class signal : virtual public object, public prioritized
    {
    
    public:
    
        object * sender;
        void * data; // TODO, avoid this void pointer
        priority_t prior;
    
        signal(     object * sender, 
                    const std::string& id = "", 
                    void * data,
                    priority_t prior = priority_t::none);

        /* Alias to flat::core::channel::emit() */
        bool emit(const std::string& channel) const;
    };

    
    /* Channel class */
    class channel : virtual public object
    {
        /* Post processing signal stacking */
        prior_set<signal> stack;
    
        /* Listeners list */
        std::list<listener*> listeners;
    
        /* Synchronous task checking for signals */
        task * checker;
        
        /* Channel mapping */
        static std::map<std::string, channel*> channels;    
     
    public:
    
        channel(const std::string& id = "", priority_t priority = priority_t::none);
        ~channel();
    
        void emit(const signal&);

        void connect(const listener*);
        void disconnect(const listener*);
       
        static channel * find_channel(const std::string&); 
    
        void post_processing();
    };
    
    /* Listener class */
    class listener : virtual public object
    {

        std::list<std::string> filters;
    
        bool check_in_filters(const std::string&) const;
    
        callback_t m_callback;

        channel * parent;
    
    public:
    
        listener(   callback_t m_callback,
                    const std::initializer_list<std::string>& filters = {});
    
        ~listener();

        void add_filter(const std::string&);
        void remove_filter(const std::string&);

        bool connect(const std::string&);
        bool disconnect(const std::string&);

        void invoke(const signal&);

        /* Allow to safely bind e functor */
        template<class T>
        static listener create(  const (T::*method(const object*, void*))& fct,
                                  T* ptr,
                                  const std::initializer_list<std::string>& filters = {})
        {
            return listener(std::bind(fct, ptr), filters);
        }

        typedef std::function<void(const object*, void*)> callback_t;
    };

    }
}

