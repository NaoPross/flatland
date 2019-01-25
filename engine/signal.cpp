#include "core/signal.hpp"
#include <functional>
#include "flatland.hpp"

using namespace std;
using namespace flat::core;

map<string, weak_ptr<channel>> channel::channels;    

channel::channel(const string& id) : labelled(id), mapped(false)
{
}

channel::~channel()
{
    // by default it should be there
    if (mapped)
        channels.erase(label);
}

void channel::start(priority_t prior)
{
    // Initialize task
    checker = flat::game_job().delegate_task(&channel::check_and_call, *this, prior);
}

bool channel::map()
{
    if (!mapped) {

        channel::ptr other = channel::find(label);

        if (!other) {

            channels.insert(pair<string, weak_ptr<channel>>(label, weak_from_this())); 
            mapped = true;
        }
    }

    return mapped;
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
        auto pt = lis.lock();

        if (pt.get() == l.get())
            return false;
    }

    listeners.push_back(l);
    return true;
}

void channel::disconnect(listener::ptr l)
{
    listeners.remove_if(
                [l](weak_ptr<listener> p){ 
                    
                    listener::ptr pt = p.lock();
                    return pt.get() == l.get(); 
                });
}

bool channel::connect(listener* l)
{
    listener::ptr pt(l);
    return connect(pt);
}

void channel::disconnect(listener* l)
{
    listener::ptr pt(l);
    disconnect(pt);
}

channel::ptr channel::create(const string& id, priority_t prior)
{
    ptr out = std::make_shared<channel>(id);

    if (!out->map())
        return nullptr;

    out->start(prior);

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
                    pt->invoke(sig);
            }
            else
                to_erase.push_back(l);
        }
    
        /* Erase invalidated listeners */
        listeners.remove_if(
            [](weak_ptr<listener> e) { return e.expired(); });

        stack.clear(); // TODO not so efficient
    }
}


/* signal class */

signal::signal( object *sender, 
                const string& id, 
                void *data, 
                priority_t priority)

    :   labelled(id, true), 
        prioritized(priority),
        sender(sender), 
        m_package(package(data))
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

listener::listener(callback m_callback, const initializer_list<string>& filters)

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

    return bool(c);
}

bool listener::disconnect(const string& chan)
{
    channel::ptr c = channel::find(chan);

    if (!c)
        c->disconnect(this);

    return bool(c);
}

void listener::invoke(const signal& sig)
{
    if (    (!sig.label.empty() && check_in_filters(sig.label)) || 
            (sig.label.empty() && filters.empty()))
        m_callback(sig.sender, sig.m_package);
}

