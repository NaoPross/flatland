#pragma once

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include "task.hpp"
#include "types.hpp"
#include "object.hpp"
#include <functional>
#include <memory>
#include "priority.hpp"
#include "labelled.hpp"

namespace flat::core
{
    class signal : virtual public labelled, virtual public prioritized
    {
    
    public:

        struct package
        {
            package(void *data) : data(data) {}

            template<class T>
            T * get() {

                return reinterpret_cast<T*>(data);
            }

            void * data;
        };
    
        object *m_sender;
        package m_package;
    
        signal(object * sender, 
               const std::string& id = "", 
               void * data = 0,
               priority_t prior = priority_t::none);
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

        std::list<std::string> m_filters;
    
        bool check_in_filters(const std::string&) const;
    };

    /* Channel class */
    class channel : virtual public labelled, public std::enable_shared_from_this<channel>
    {
    private:
        /* Post processing signal stacking */
        queue<signal> m_stack;
    
        /* Listeners list */
        std::list<std::weak_ptr<listener>> m_listeners;
    
        /* Synchronous task checking for signals */
        task::ptr m_checker;
        
        /* Channel mapping */
        static std::map<std::string, std::weak_ptr<channel>> m_channels;

        bool m_mapped;

        bool map();
     
    public:

        using ptr = std::shared_ptr<channel>;
   
        /* 
         * Constructs a new channel and try to bind it the specified job
         * If no job is specified, then flat::main_job will be taken
         *
         * Note: Remember to check for channel legacy with legit() member function
         *       A channel is legit if and only if there's no other channel
         *       corresponding to the same label name
         *       In general, a channel must be UNIQUE with its label
         */ 
        channel(const std::string& id = "");
        ~channel();

        // do not allow to copy a channel
        // it does not have any sense to create a channel with the same name
        channel(const channel&) = delete;
        channel& operator=(const channel&) = delete;
    
        void emit(const signal&);

        bool connect(listener::ptr l);
        void disconnect(listener::ptr l);

        bool connect(listener* l);
        void disconnect(listener* l);

        /* 
         * Check for legacy
         *
         * Note: Channels are mapped by their label.
         *       A channel is legit if and only if there's no other channel
         *       corresponding to the same label name.
         */
        bool legit() const;

        /*
         * It tries binding the channel task and make the channel legit
         * In case of success, true is returned, otherwise false
         */
        bool start(priority_t task_priority = priority_t::none, job * _job = NULL);

        listener::ptr connect(listener::callback f,
            const std::initializer_list<std::string>& filters = {});

        template<typename R, typename T>
        inline listener::ptr connect(R T::*mf, T* obj,
            const std::initializer_list<std::string>& filters = {})
        {
            using namespace std::placeholders;
            return connect(std::bind(mf, obj, _1, _2), filters);
        }
      
        /* 
         * Find channel by its name
         */ 
        static ptr find(const std::string&); 

        /*
         * Safer than constructor
         * It constructs a new channel checking whether it is legit or not.
         * It returns a nullptr if the legacy is not verified.
         *
         * Note: Channels are mapped by their label.
         *       A channel is legit if and only if there's no other channel
         *       corresponding to the same label name.
         */
        static ptr create(const std::string& id, priority_t prior = priority_t::none, job * _job = NULL);
    
        void check_and_call();
    };
}

