#include "core/signal.hpp"
#include "object.hpp"
#include <functional>
#include "flatland.hpp"

#include "debug.hpp"

using namespace flat::core;

std::map<std::string, std::weak_ptr<channel>> channel::channels;    

channel::channel(const std::string& id) : labelled(id), mapped(false)
{
}

channel::~channel()
{
    // by default it should be there
    if (mapped)
        channels.erase(label);
}

bool channel::start(priority_t prior, job * _job)
{
    npdebug("Trying to map channel: ", label);

    if (map())
        checker.reset(); // force count to zero, it should remove task too
    else
        return false;

    npdebug("Starting channel: ", label);

    // take main job if no job is specified
    if (!_job)
        _job = &flat::main_job();

    // Initialize task
    checker = _job->delegate_task(&channel::check_and_call, this, prior);

    return true;
}

void channel::finalize()
{
    npdebug("Finalizing channel: ", label)
    
    if (mapped) {

        channels.erase(label);
        checker.reset();
    }
}

bool channel::map()
{
    if (!mapped) {

        channel::ptr other = channel::find(label);

        if (!other) {

            channels.insert(std::pair<std::string, std::weak_ptr<channel>>(label, weak_from_this())); 
            mapped = true;
        }
    }

    return mapped;
}

bool channel::connect(std::shared_ptr<abstract_listener> l)
{
    /* Control not to double */
    for (auto lis : listeners)
    {
        auto pt = lis.lock();

        if (pt.get() == l.get())
            return false;
    }

    listeners.push_back(l);
    return true;
}

bool channel::legit() const
{
    return mapped;
}

channel::ptr channel::create(const std::string& id, priority_t prior, job * _job)
{
    ptr out = std::make_shared<channel>(id);

    // do not create a non-legit channel
    return out->start(prior, _job) ? out : nullptr;
}
   
channel::ptr channel::find(const std::string& id)
{
    if (id.empty())
        return nullptr;

    auto it = channels.find(id);

    return (it == channels.end()) ? nullptr : (*it).second.lock();
}

void channel::check_and_call()
{
    if (!stack.empty()) {

        npdebug("Signal detected: ", label, " ", this)

        std::vector<std::weak_ptr<abstract_listener>> to_erase;

        // TODO, maybe it exists pop
        /* for each listener_s, catch signal */

        for (auto l : listeners)
        {
            std::shared_ptr<abstract_listener> pt;

            if (pt = l.lock())
            {
                for (const auto& sig : stack)
                {
                    // pass simple pointer
                    pt->invoke(sig.get());
                }
            }
            else
                to_erase.push_back(l);
        }
    
        /* Erase invalidated listeners */
        listeners.remove_if(
            [](std::weak_ptr<abstract_listener> e) { return e.expired(); });

        stack.clear(); // TODO not so efficient
    }
}


/* signal class */

abstract_signal::abstract_signal(const std::string& id, priority_t priority)

    :   labelled(id, true), 
        prioritized(priority),
{
}

/*bool signal::emit(const std::string& chan) const
{
    channel::ptr c = channel::find(chan);
    
    if (!c)
        return false;
    
    c->emit(*this);

    return true;
}*/


/* listener_s class */

abstract_listener::abstract_listener(const std::initializer_list<std::string>& filters)

    : filters(filters)
{

}

abstract_listener::~abstract_listener()
{
}

bool abstract_listener::check_in_filters(const std::string& filter) const
{
    for (const auto& f : filters)
    {
        if (filter == f)
            return true;
    }

    return false;
}

void abstract_listener::add_filter(const std::string& f)
{
    filters.push_back(f);
}

void abstract_listener::remove_filter(const std::string& f)
{
    filters.remove(f);
}

bool abstract_listener::match_filters(const abstract_signal *sig) const
{
    return (!sig.label.empty() && check_in_filters(sig.label)) || 
           (sig.label.empty() && filters.empty());
}

