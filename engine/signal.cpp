#include "core/signal.hpp"
#include "flatland.hpp"
#include "debug.hpp"

#include <functional>

using namespace flat::core;

/* channel */

channel::channel(priority_t p) : channel(flat::main_job(), p) {}

channel::channel(job& broadcaster, priority_t p) 
{
    m_broadcast = broadcaster->delegate_task(&channel::broadcast, this, p);
}

void channel::broadcast()
{
    npdebug("broadcasting signals from channel ", this);
    std::list<std::weak_ptr<helper::listener>> to_erase;

    for (auto&& sig : m_signals) {
        bool invoked = false;

        for (auto&& l : m_listeners) {
            if (std::shared_ptr<helper::listener> pt = l.lock()) {
                if (pt->invoke(sig))
                    invoked = true;
            } else {
                npdebug("found an expired listener");
                to_erase.push_front(l);
            }
        }

        if (!invoked)  {
            npdebug("signal ", sig, " was ignored (not invoked)");
        }
    }

    // TODO: check if this works
    for (auto&& l : to_erase) {
        // because weak_ptr do not have operator==
        m_listeners.remove_if([&](const auto& v) {
            return std::owner_less<std::weak_ptr<helper::listener>>()(l, v);
        });
    }

    // release all accumulated signals
    m_signals.clear();
}
