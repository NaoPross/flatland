#include "core/signal.hpp"
#include <functional>
#include "flatland.h"

using namespace std;
using namespace flat::core;

map<string, channel*> channel::channels;    

channel::channel(const string& id, priority_t prior) : labelled(id)
{
}

channel::~channel()
{
    // by default it should be there
    channels.erase(label);
}

void channel::start()
{
    // Initialize task
    checker = flat::game_job().delegate_task<channel>(&channel::check_and_call, this, prior);
}

bool channel::map()
{
    channel * other = find_channel(label);

    return other ? false : channels.insert(pair<string, channel*>(label, this));
}

void channel::emit(const signal& sig)
{
    stack.insert(sig);
}

bool channel::connect(listener::ptr l)
{
    /* Control not to double */
    for (auto lis : listeners)
    {
        if (lis.get() == l.get())
            return false;
    }

    listeners.push_back(l);
    return true;
}

void channel::disconnect(listener::ptr l)
{
    listeners.remove(l);
}

channel::ptr channel::create(const string& id, priority_t prior)
{
    ptr out = std::make_ptr(new channel(id, prior));

    if (!out.map())
        return nullptr;

    out.start();

    return out;
}
   
channel::ptr channel::find(const string& id)
{
    if (id.empty())
        return nullptr;

    auto it = channels.find(id);

    return (it == channels.end()) ? nullptr : (*it).second.lock();
}

void channel::check_and_call()
{
    if (!stack.empty()) {

        vector<weak_ptr<listener>> to_erase;

        // TODO, maybe it exists pop
        /* for each listener_s, catch signal */

        for (auto l : listeners)
        {
            listener::ptr pt;

            if (pt = l.lock())
            {
                for (const auto& sig : stack)
                    l->invoke(sig);
            }
            else
                to_erase.push_back(l);
        }
    
        /* Erase invalidated listeners */
        for (auto e : to_erase)
            listeners.remove(e);

        stack.clear(); // TODO not so efficient
    }
}


/* signal class */

signal::signal( object *sender, 
                const string& id, 
                void *data, 
                priority_t priority)

    :   labelled(id, true), 
        sender(sender), 
        m_package(package(data)), 
        priority(priority)
{
}

bool signal::emit(const string& chan) const
{
    channel::ptr c = channel::find(chan);
    
    if (!c)
        return false;
    
    /* Finally emit in channel */
    c->emit(*this);

    return true;
}


/* listener_s class */

listener::listener(   callback_t m_callback,
                      const initializer_list<string>& filters)

    : m_callback(m_callback), filters(filters)
{

}

listener::~listener()
{
}

bool listener::check_in_filters(const string& filter) const
{
    for (const auto& f : filters)
    {
        if (filter == f)
            return true;
    }

    return false;
}

void listener::add_filter(const string& f)
{
    filters.push_back(f);
}

void listener::remove_filter(const string& f)
{
    filters.remove(f);
}

bool listener::connect(const string& chan)
{    
    channel::ptr c = channel::find(chan);

    if (!c)
        c->connect(this);

    return c;
}

bool listener::disconnect(const string& chan)
{
    channel::ptr c = channel::find_channel(chan);

    if (!c)
        c->disconnect(this);

    return c;
}

void listener::invoke(const signal& sig)
{
    if (    (!sig.label.empty() && check_in_filters(sig.label)) || 
            (sig.label.empty() && filters.empty()))
        m_callback(sig.sender, sig.m_package);
}

