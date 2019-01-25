#pragma once

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include "task.hpp"
#include "types.hpp"
#include <functional>
#include <memory>
#include "priority.hpp"
#include "labelled.hpp"

namespace flat
{
    class object;

    namespace core
    {

    class signal : virtual public labelled, virtual public prioritized
    {
    
    public:

        struct package
        {
            package(void *data) : data(data) {}

            template<class T>
            T * get() {

                return dynamic_cast<T>(data);
            }

            void * data;
        };
    
        object * sender;
        package m_package;
    
        signal(     object * sender, 
                    const std::string& id = "", 
                    void * data = 0,
                    priority_t prior = priority_t::none);

        /* Alias to flat::core::channel::emit() */
        bool emit(const std::string& channel) const;
    };
        
    /* Listener class */
    class listener
    {
    public:

        using callback = std::function<void(const object*, signal::package)>;
        using ptr = std::shared_ptr<listener>;

        listener(callback m_callback, const std::initializer_list<std::string>& filters = {});
        ~listener();

        void add_filter(const std::string&);
        void remove_filter(const std::string&);

        bool connect(const std::string&);
        bool disconnect(const std::string&);

        void invoke(const signal&);

        /* Allow to safely bind a functor */
        template<typename R, typename T>
        static ptr create(  R T::*mf,
                            T& obj,
                            const std::initializer_list<std::string>& filters = {}) {
            return std::make_shared<listener>(std::bind(mf, obj), filters);
        }

    private:

        callback m_callback;

        std::list<std::string> filters;
    
        bool check_in_filters(const std::string&) const;
    };

    /* Channel class */
    class channel : virtual public labelled, public std::enable_shared_from_this<channel>
    {
        /* Post processing signal stacking */
        queue<signal> stack;
    
        /* Listeners list */
        std::list<std::weak_ptr<listener>> listeners;
    
        /* Synchronous task checking for signals */
        task::ptr checker;
        
        /* Channel mapping */
        static std::map<std::string, std::weak_ptr<channel>> channels;    

        bool mapped;
     
    public:

        using ptr = std::shared_ptr<channel>;
    
        channel(const std::string& id = "");
        ~channel();

        void start(priority_t task_priority = priority_t::none);
        bool map();
    
        void emit(const signal&);

        bool connect(listener::ptr l);
        void disconnect(listener::ptr l);

        bool connect(listener* l);
        void disconnect(listener* l);

        listener::ptr connect(listener::callback f, const std::initializer_list<std::string>& filters = {});

        template<typename R, typename T>
        inline listener::ptr connect(R T::*mf, T& obj, const std::initializer_list<std::string>& filters = {}) {
            return connect(std::bind(mf, obj));
        }
       
        static ptr find(const std::string&); 

        static ptr create(const std::string& id, priority_t prior = priority_t::none);
    
        void check_and_call();
    };


    }
}

