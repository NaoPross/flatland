#include "core/signal.hpp"
#include "object.hpp"
#include <functional>
#include "flatland.hpp"

#include "debug.hpp"

using namespace flat::core;

/* listener */

template<typename F, typename ...Args>
bool listener<F, Args ...>::invoke(std::shared_ptr<const helper::signal> s)
{
    const signal<Args...> *p = dynamic_cast<const signal<Args...> *>(s.get());

    if (p != nullptr)
        std::apply(m_callback, *p);

    // return true if p was called
    return (p != nullptr);
}


/* channel */

channel::channel(job& broadcaster) 
{
    m_broadcast = broadcaster.delegate_task(&channel::broadcast, this);
}

void channel::broadcast()
{
    std::vector<std::weak_ptr<helper::listener>> to_erase;

    for (const auto& sig : m_signals) {
        for (auto l : m_listeners) {
            // check that is not expired
            std::shared_ptr<helper::listener> pt;

            if ((pt = l.lock())) {
                pt->invoke(sig);
            } else {
                to_erase.push_back(l);
            }
        }
    }
}
