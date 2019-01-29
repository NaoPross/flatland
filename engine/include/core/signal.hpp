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
#include <cstddef>
#include <type_traits>
#include "priority.hpp"
#include "labelled.hpp"

/*
 * Some tools for variadic binding templates
 */
 
namespace helper
{
        
    template<int...> struct int_sequence {};

    template<int N, int... Is> struct make_int_sequence
        : make_int_sequence<N-1, N-1, Is...> {};
    template<int... Is> struct make_int_sequence<0, Is...>
        : int_sequence<Is...> {};

    template<int>
    struct placeholder_template {};
}

namespace std
{
    template<int N>
    struct is_placeholder< helper::placeholder_template<N> >
        : integral_constant<int, N+1> // the one is important
    {};
}

namespace flat
{
    //class object;

    namespace core
    {

    /* 
     * Signal class
     */

    struct abstract_signal : public labelled, public prioritized
    {
        abstract_signal(const std::string& id = "", priority_t prior = priority_t::none);

        // virtual destructor, sure to call right class destructor
        virtual ~abstract_signal() {}
    };

    template <class ...Args> 
    struct signal : public abstract_signal
    {
    
        std::tuple<Args...> args;

        signal(     Args&& ... args,
                    const std::string& id = "", 
                    priority_t prior = priority_t::none)

            : abstract_signal(id, prior), args(std::forward<Args>(args)...)
        {

        }

        /* Alias to flat::core::channel::emit() */
        //bool emit(const std::string& channel) const;
    };

    class abstract_listener
    {
        std::list<std::string> filters;

    protected:
    
        bool check_in_filters(const std::string&) const;

        bool match_filters(const abstract_signal *sig) const;

    public:

        abstract_listener(const std::initializer_list<std::string>& filters = {});
        virtual ~abstract_listener();

        void add_filter(const std::string&);
        void remove_filter(const std::string&);

        virtual void invoke(const abstract_signal *) = 0;
    };
        
    /* Listener class */
    template <class ...Args>
    class listener : public abstract_listener
    {

    public:

        using callback = typename std::function<void(Args...)>;
        using ptr = typename std::shared_ptr<listener<Args...>>;

        listener(callback m_callback, const std::initializer_list<std::string>& filters = {})
            : abstract_listener(filters), m_callback(m_callback)
        {

        }

        //bool connect(const std::string&);
        //bool disconnect(const std::string&);

        template<int ...Is>
        void invoke(const abstract_signal * sig, helper::int_sequence<Is...>)
        {
            const signal<Args...> * pt = dynamic_cast<const signal<Args...>*>(sig);

            // check if the arguments and the filters match
            if (pt && match_filters(sig))
                m_callback(std::get<Is>(pt->args)...);
        }

        // implement base class method
        virtual bool invoke(const abstract_signal * sig) override
        {
            return invoke(sig, helper::make_int_sequence<sizeof...(Args)>{});
        }

    private:

        callback m_callback;
    };

    
    /* Channel class */
    class channel : virtual public labelled, public std::enable_shared_from_this<channel>
    {
        /* Post processing signal stacking */
        queue<std::unique_ptr<abstract_signal>> stack;
    
        /* Listeners list */
        std::list<std::weak_ptr<abstract_listener>> listeners;
    
        /* Synchronous task checking for signals */
        task::ptr checker;
        
        /* Channel mapping */
        static std::map<std::string, std::weak_ptr<channel>> channels;    

        bool mapped;

        bool map();

        bool connect(std::shared_ptr<abstract_listener> l);
             
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
   
        template<class ...Args> 
        void emit(const signal<Args...>& sig)
        {   
            stack.insert(std::make_unique<abstract_signal>(sig));
            npdebug("Emitted signal: ", sig.label, " ", this);
        }
        

        template<class ...Args> 
        void disconnect(typename listener<Args...>::ptr l)
        {
            listeners.remove_if(
                [l](std::weak_ptr<abstract_listener> p){ 
                    
                    typename listener<Args...>::ptr pt = p.lock();
                    return pt.get() == l.get(); 
                });
        }

        template<class ...Args> 
        void disconnect(listener<Args...>* l)
        {
            typename listener<Args...>::ptr pt(l);
            disconnect<Args...>(pt);
        }

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

        /* 
         * Successfully stops the channel
         * Any related job is then detached
         */
        void finalize();

        /* 
         * Connects any bound function to the channel and 
         * returns the corresponding listener
         */
        template<class ...Args>
        typename listener<Args...>::ptr connect( typename listener<Args...>::callback f,
                                        const std::initializer_list<std::string>& filters = {})
        {
            typename listener<Args...>::ptr lis = std::make_shared<listener<Args...>>(f, filters);

            if (connect(std::static_pointer_cast<abstract_listener>(lis)))
                return lis;

            return nullptr;
        }

        /*
         * Connect a class member function and returns the
         * corresponding listener
         */
        template<typename R, typename T, class ...Args>
        inline typename listener<Args...>::ptr connect(R T::*mf, T* obj, const std::initializer_list<std::string>& filters = {})
        {
            //using namespace std::placeholders;
            //return connect<Args...>(std::bind(mf, obj, _1, _2), filters);
            using namespace helper;
            return connect<Args...>(std::bind(mf, obj, make_int_sequence<sizeof...(Args)>{}), filters);
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
}

