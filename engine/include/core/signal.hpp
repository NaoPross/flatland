#pragma once

#include "task.hpp"
#include "types.hpp"
#include "object.hpp"
#include "priority.hpp"
#include "labelled.hpp"
#include "debug.hpp"

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include <functional>
#include <memory>
#include <cstddef>
#include <type_traits>

/*
 * Some tools for variadic binding templates
 */

namespace flat::core
{
    /* forward decls */
    template<typename F, typename ...Args>
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
            signal(const signal& other) = delete;
            signal& operator=(const signal& other) = delete;

        protected:
            signal(priority_t p) : prioritized(p) {}
        };
    }

    /* class signal<...Args>
     *
     * is a specialized tuple that contains funnction arguments (...Args) 
     * that are later used to calll a callback stored in a listener.
     */
    template <typename ...Args> 
    class signal : private std::tuple<Args...>, public helper::signal
    {
    private:
        /// listener with the same signature is allowed to peek
        friend class listener<Args...>;
        
    public:
        signal(Args&&... args, priority_t p = priority_t::none) 
            : std::tuple<Args...>(std::forward<Args>(args)...),
              helper::signal(p) {}
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
        private:
            listener(const signal& other) = delete;
            listener& operator=(const listener& other) = delete;

        protected:
            listener() {}

        public:
            virtual bool invoke(std::shared_ptr<const helper::signal> s) = 0;
        };
    }
        
    /* class listener<F, ...Args>
     *
     * is an object holding a callback, that can be only called by passing
     * a signal<...Args> object, which contains the arguments for the callback.
     */
    template <typename F, typename ...Args>
    class listener : public helper::listener 
    {
    public:
        using callback = typename std::function<void(Args...)>;
        using ptr = typename std::shared_ptr<listener<Args...>>;

        listener() = delete;
        listener(F&& f) : m_callback(std::move(f)) {}

        // attempt to call m_callback with s as argument
        // m_callback is called only if the signature matches
        bool invoke(std::shared_ptr<const helper::signal> s) override;

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
        std::set<std::weak_ptr<helper::listener>> m_listeners;
        queue<std::shared_ptr<helper::signal>> m_signals;

        /// task to call 
        std::shared_ptr<task> m_broadcast;
             
    public:
        using ptr = std::shared_ptr<channel>;
   
        // TODO: channel() that binds to main_job
        channel(job& broadcaster);

        // do not allow to copy a channel
        // it does not make sense to create a channel with the same name
        channel(const channel&) = delete;
        channel& operator=(const channel&) = delete;
   
        /// add a signal to the queue/stack of signals (m_signals)
        template<class ...Args> 
        void emit(const signal<Args...>& sig)
        {   
            // insert signal and decay it to a non-template
            //   unique_ptr<helper::signal>
            m_signals.insert(std::make_unique<helper::signal>(sig));
        }

        /// for each signal accumulated, call each listener
        void broadcast();

        /// connect a standalone function
        template<typename F, typename ...Args>
        std::shared_ptr<listener<F, Args...>> connect(listener<F, Args...>::callback f)
        {
            auto lis_ptr = std::make_shared<listener<Args...>>(f);
            // insert listener and decay it to a non-template
            //   weak_ptr<helper::listener>
            m_listeners.insert(lis_ptr);

            return lis_ptr;
        }

        /// connect a member function
        template<typename R, typename T, typename F, typename ...Args>
        std::shared_ptr<listener<F, Args...>> connect(R T::*mf, T& obj)
        {
            return connect([mf](Args&& ...args) -> R {
                return (mf(args),...);
            });
        }
    };
}

