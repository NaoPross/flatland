#pragma once

#include "task.hpp"
#include "types.hpp"
#include "priority.hpp"
#include "labelled.hpp"
#include "debug.hpp"

#include <tuple>
#include <list>
#include <functional>
#include <memory>


namespace flat::core
{
    /* forward decls */
    template<typename ...Args>
    class listener;

    class channel;

    namespace helper
    {
        /* class helper::signal 
         *
         * is a non constructible, but copyable object used only as abstract
         * pointer type. To make anything with it must be first downcasted to a
         * core::signal<Args...>
         */
        class signal : public prioritized
        {
        public:
            /// non copyable
            signal(const signal& other) = delete;
            /// movable
            signal(signal&& other) = default;

            virtual ~signal() {}

        protected:
            /// not default constructible
            signal() = delete;

            /// normal constructor, allowed only by core::signal
            signal(priority_t p = priority_t::none) : prioritized(p) {}
        };
    }

    /* class signal<...Args>
     *
     * is a tuple wrapper that contains function arguments (...Args) 
     * that are later used to call a callback stored in a listener.
     */
    template <typename ...Args> 
    struct signal : public helper::signal
    {
        const std::tuple<Args...> args;

        /// disallow empty constructor
        // TODO: think: should empty signals be allowed?
        //       yes if there is a way to identify them by something that
        //       is not the signature of the listener (so not yet)
        signal() = delete;

        /// non copyable, copying signal<...>::args is inefficient
        signal(const signal& other) = delete;

        /// movable
        signal(signal&& other) = default;

        constexpr signal(const Args&... _args)
            : helper::signal(priority_t::none), args(_args...) {}

        /// normal (inefficient) constructor that copies arguments
        constexpr signal(priority_t p, const Args&... _args)
            : helper::signal(p), args(_args...) {}

        // constexpr signal(priority_t p, const Args&... _args)
        //     : helper::signal(p), args(_args...) {}

        /// normal constructor that forwards arguments
        /// this optimizes rvalue initializations
        constexpr signal(Args&&... _args)
            : helper::signal(priority_t::none),
              args(std::forward<Args>(_args)...)
        {}

        // constexpr signal(priority_t p, Args&&... _args)
        //     : helper::signal(p),
        //       args(std::forward<Args>(_args)...)
        // {}
    };


    namespace helper
    {
        /* class helper::listener
         *
         * is a non constructible, but copyable object used only as abstract
         * pointer type. To make anything with it must be first downcasted to a
         * core::listener<Args...>
         */
        class listener
        {
        public:
            /// copyable
            listener(const listener& other) = default;

            /// movable
            listener(listener&& other) = default;

            virtual ~listener() {}

            /// pure interface
            virtual bool invoke(std::shared_ptr<const helper::signal> s) const = 0;

        protected:
            /// constructuble only by listener
            listener() = default;
        };
    }
        
    /* class listener<F, ...Args>
     *
     * is an object holding a callback, that can be only called by passing
     * a signal<...Args> object, which contains the arguments for the callback.
     *
     * Note: listener objects can be created only by a channel.
     */
    template <typename ...Args>
    class listener : public helper::listener
    {
    public:
        using callback = typename std::function<void(Args...)>;

        friend class channel;

        /// not default constructible
        listener() = delete;

        /// copyable
        listener(const listener&) = default;

        /// movable
        listener(listener&&) = default;

        /// attempt to call m_callback with s as argument
        /// m_callback is called only if the signature matches
        bool invoke(std::shared_ptr<const helper::signal> s) const override
        {
            auto p = std::dynamic_pointer_cast<const signal<Args...>>(s);

            // if dynamic cast fails
            if (!p) {
                npdebug("invoked listener ", this, " with non-matching signal ", s);
                return false;
            }

            npdebug("invoked listener ", this, " with signal ", p);
            std::apply(m_callback, (p->args));

            return true;
        }

    private:
        /// normal constructor only allowed by channel
        listener(callback f) : m_callback(f) {}

        callback m_callback;
    };

    
    /* class channel
     *
     * is an object type through which signals are emitted.
     * and is an object type through which listener get their signals.
     */
    class channel : virtual public labelled
    {
    private:
        // this is a set because sets do not allow identical elements
        std::list<std::weak_ptr<helper::listener>> m_listeners;
        queue<std::shared_ptr<helper::signal>> m_signals;

        /// task to call 
        std::shared_ptr<task> m_broadcast;

        /// connect a std::function (this is a helper), see others below
        template<typename R, typename ...Args>
        std::shared_ptr<listener<Args...>> _connect(std::function<R(Args...)>&& f)
        {
            // construct a listener by forwarding f to listener's constructor
            auto lis_ptr = std::make_shared<listener<Args...>>(
                listener<Args...>(std::forward<decltype(f)>(f))
            );

            // insert pointer
            m_listeners.push_front(
                // decay shared_ptr to weak_ptr
                //   btw, here a static_cast is correct
                static_cast<std::weak_ptr<helper::listener>>(
                    // decay listener to helper::listener
                    std::static_pointer_cast<helper::listener>(lis_ptr)
                )
            );

            return lis_ptr;
        }
             
    public:
        using ptr = std::shared_ptr<channel>;
   
        // channel(priority_t) that binds to main_job
        channel(priority_t p = priority_t::none);

        channel(job& broadcaster, priority_t p = priority_t::none);

        /// not copyable
        // TODO: review: should be copyable?
        channel(const channel&) = delete;

        /// movable
        channel(channel&&) = default;

        /// add a signal to the queue/stack of signals (m_signals)
        template<class ...Args> 
        void emit(signal<Args...>&& sig)
        {   
            // create a shared_ptr
            auto p = std::make_shared<signal<Args...>>(
                std::forward<signal<Args...>>(sig)
            );

            npdebug("emitted signal ", p);

            // insert pointer
            m_signals.insert(
                // decay signal to helper::signal
                std::static_pointer_cast<helper::signal>(p)
            );
        }

        /// for each signal accumulated, call each listener
        void broadcast();

        /// connect a closure
        // template<typename ...Args, typename Closure>
        // std::shared_ptr<listener<Args...>> connect(Closure f)
        // {
            // TODO: fix
        // }

        /// connect a function
        template<typename R, typename ...Args>
        std::shared_ptr<listener<Args...>> connect(R (*f)(Args...))
        {
            return _connect(static_cast<std::function<R(Args...)>>(
                // closure that forwards ...args to f
                [f](Args&& ...args) constexpr -> R {
                    return f(std::forward<Args>(args)...);
                })
            );
        }

        /// connect a member function
        template<typename R, typename T, typename ...Args>
        std::shared_ptr<listener<Args...>> connect(R (T::*mf)(Args ...args), T* obj)
        {
            return _connect(static_cast<std::function<R(Args...)>>(
                // closure that forwards ...args to mf called on obj
                [mf, obj](Args&& ...args) constexpr -> R {
                    return (obj->*mf)(std::forward<Args>(args)...);
                })
            );
        }
    };
}

