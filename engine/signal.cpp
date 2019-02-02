#include "core/signal.hpp"
#include "object.hpp"
#include "flatland.hpp"
#include "debug.hpp"

#include <functional>

using namespace flat::core;

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
