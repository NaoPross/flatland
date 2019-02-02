#pragma once

#include "task.hpp"
#include "types.hpp"
#include "priority.hpp"
#include "labelled.hpp"
#include "debug.hpp"

#include <map>
#include <list>
#include <tuple>
#include <functional>
#include <memory>
#include <cstddef>
#include <type_traits>


namespace flat::core
{
    /* forward decls */
    template<typename ...Args>
    class listener;

    namespace helper
    {
        /* class helper::signal 
         *
         * is a non constructible, non copyable object used only as abstract
         * pointer type. To make anything with it must be first downcasted to a
         * core::signal<Args...>
         */
        class signal : public prioritized
        {
        public:
            virtual ~signal() {}

        protected:
            explicit signal(priority_t p = priority_t::none) : prioritized(p) {}
        };
    }

    /* class signal<...Args>
     *
     * is a specialized tuple that contains function arguments (...Args) 
     * that are later used to call a callback stored in a listener.
     */
    template <typename ...Args> 
    struct signal : public helper::signal
    {
        const std::tuple<Args...> args;

        explicit constexpr signal(Args... _args)
            : args(std::move(_args)...) {}
    };


    namespace helper
    {
        /* class helper::listener
         *
         * is a non constructible, non copyable object used only as abstract
         * pointer type. To make anything with it must be first downcasted to a
         * core::listener<Args...>
         */
        class listener
        {
        protected:
            explicit listener() {}
            virtual ~listener() {}

        public:
            virtual bool invoke(std::shared_ptr<const helper::signal> s) const = 0;
        };
    }
        
    /* class listener<F, ...Args>
     *
     * is an object holding a callback, that can be only called by passing
     * a signal<...Args> object, which contains the arguments for the callback.
     */
    template <typename ...Args>
    class listener : public helper::listener
    {
    public:
        using callback = typename std::function<void(Args...)>;
        using ptr = typename std::shared_ptr<listener<Args...>>;

        explicit listener(callback f) : m_callback(f) {}

        // attempt to call m_callback with s as argument
        // m_callback is called only if the signature matches
        bool invoke(std::shared_ptr<const helper::signal> s) const override
        {
            const signal<Args...> *p = dynamic_cast<const signal<Args...> *>(s.get());

            if (p != nullptr)
                std::apply(m_callback, p->args);

            // return true if p was called
            return (p != nullptr);
        }

    private:
        callback m_callback;
    };

    
    /* class channel
     *
     * is an object type through which signals are emitted.
     * and is an object type through which listener get their signals.
     */
    class channel : virtual public labelled, public std::enable_shared_from_this<channel>
    {
    private:
        // this is a set because sets do not allow identical elements
        std::list<std::weak_ptr<helper::listener>> m_listeners;
        queue<std::shared_ptr<helper::signal>> m_signals;

        /// task to call 
        std::shared_ptr<task> m_broadcast;
             
    public:
        using ptr = std::shared_ptr<channel>;
   
        // TODO: channel() that binds to main_job
        explicit channel(job& broadcaster);

        /// add a signal to the queue/stack of signals (m_signals)
        template<class ...Args> 
        void emit(const signal<Args...> sig)
        {   
            // insert signal and decay it to a non-template
            //   unique_ptr<helper::signal>
            m_signals.insert(std::make_shared<helper::signal>(sig));
        }

        /// for each signal accumulated, call each listener
        void broadcast();

        /// connect a standalone function
        template<typename R, typename ...Args>
        std::shared_ptr<listener<Args...>> _connect(std::function<R(Args...)> f)
        {
            auto lis_ptr = std::make_shared<listener<Args...>>(f);
            // insert listener and decay it to a non-template
            //   weak_ptr<helper::listener>
            m_listeners.push_back(
                static_cast<std::weak_ptr<helper::listener>>(lis_ptr)
            );

            return lis_ptr;
        }

        template<typename R, typename ...Args>
        std::shared_ptr<listener<Args...>> connect(R (*f)(Args...))
        {
            return _connect(static_cast<std::function<R(Args...)>>(
                [=](Args ...args) -> R {
                    return f((args)...);
                })
            );
        }

        /// connect a member function
        template<typename R, typename T, typename ...Args>
        std::shared_ptr<listener<Args...>> connect(R (T::*mf)(Args ...args), T* obj)
        {
            return _connect(static_cast<std::function<R(Args...)>>(
                [=](Args ...args) -> R {
                    return (obj->*mf)((args)...);
                })
            );
        }
    };
}


namespace std
{
    template<typename... Args>
    struct tuple_size<flat::core::signal<Args...>>
    {
        constexpr static std::size_t value = (0 + ... + sizeof(Args));
    };
}
