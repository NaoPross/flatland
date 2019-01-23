#pragma once

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include "object.hpp"
#include "task.hpp"
#include "types.h"
#include <functional>
#include <memory>
#include "priority.hpp"
#include "labelled.hpp"

namespace flat
{
    namespace core
    {

    class signal : virtual public labelled, virtual public prioritized
    {
    
    public:
    
        object * sender;
        package m_package;
        priority_t prior;
    
        signal(     object * sender, 
                    const std::string& id = "", 
                    void * data = 0,
                    priority_t prior = priority_t::none);

        /* Alias to flat::core::channel::emit() */
        bool emit(const std::string& channel) const;

        struct package
        {
            package(void *data) : data(data) {}

            template<class T>
            T * get() {

                return dynamic_cast<T>(data);
            }

            void * data;
        };
    };

    
    /* Channel class */
    class channel : virtual public labelled
    {
        /* Post processing signal stacking */
        queue<signal> stack;
    
        /* Listeners list */
        std::list<std::weak_ptr<listener>> listeners;
    
        /* Synchronous task checking for signals */
        task::ptr checker;
        
        /* Channel mapping */
        static std::map<std::string, std::weak_ptr<channel>> channels;    
     
    public:
    
        channel(const std::string& id = "", priority_t task_priority = priority_t::none);
        ~channel();

        void start();
        bool map();
    
        void emit(const signal&);

        bool connect(listener::ptr l);
        void disconnect(listener::ptr l);
       
        static ptr find(const std::string&); 

        static ptr create(const string& id, priority_t prior);
    
        void check_and_call();

        typedef shared_ptr<channel> ptr;
    };
    
    /* Listener class */
    class listener
    {
        std::list<std::string> filters;
    
        bool check_in_filters(const std::string&) const;
    
        callback_t m_callback;
    
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
        static ptr make(  const (T::*method(const object*, void*))& fct,
                          T* obj,
                          const std::initializer_list<std::string>& filters = {})
        {
            return new listener(std::bind(fct, ptr), filters);
        }

        typedef std::function<void(const object*, signal::package)> callback_t;
        typedef shared_ptr<listener> ptr;
    };

    }
}

