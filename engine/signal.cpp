#include "core/signal.hpp"
#include "object.hpp"
#include <functional>
#include "flatland.hpp"

#include "debug.hpp"

using namespace flat::core;

std::map<std::string, std::weak_ptr<channel>> channel::m_channels;

channel::channel(const std::string& id) : labelled(id), m_mapped(false)
{
}

channel::~channel()
{
    // by default it should be there
    if (m_mapped)
        m_channels.erase(label);
}

void channel::start(priority_t prior)
{
    npdebug("Starting channel: ", label);

    // Initialize task
    m_checker = flat::main_job().delegate_task(&channel::check_and_call, this, prior);
}

bool channel::map()
{
    if (!m_mapped) {

        channel::ptr other = channel::find(label);

        if (!other) {
            m_channels.insert(std::pair<std::string, std::weak_ptr<channel>>(label, weak_from_this())); 
            m_mapped = true;
        }
    }

    return m_mapped;
}

void channel::emit(const signal& sig)
{
    m_stack.insert(sig);
    npdebug("Emitted signal: ", sig.label, " ", this);
}

bool channel::connect(listener::ptr l)
{
    /* Control not to double */
    for (auto lis : m_listeners)
    {
        auto pt = lis.lock();

        if (pt.get() == l.get())
            return false;
    }

    m_listeners.push_back(l);
    return true;
}

void channel::disconnect(listener::ptr l)
{
    m_listeners.erase(
        std::remove_if(
            m_listeners.begin(),
            m_listeners.end(),
            [&l](std::weak_ptr<listener> p){ 
                listener::ptr pt = p.lock();
                return pt.get() == l.get(); 
            }
        )
    );
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

listener::ptr channel::connect(listener::callback f, const std::initializer_list<std::string>& filters)
{
    listener::ptr lis = std::make_shared<listener>(f, filters);

    if (connect(lis))
        return lis;

    return nullptr;
}

channel::ptr channel::create(const std::string& id, priority_t prior)
{
    ptr out = std::make_shared<channel>(id);

    if (!out->map())
        return nullptr;

    out->start(prior);

    return out;
}
   
channel::ptr channel::find(const std::string& id)
{
    // what about anonymous (unlabeled) channels?
    if (id.empty())
        return nullptr;

    auto it = m_channels.find(id);

    return (it == m_channels.end()) ? nullptr : (*it).second.lock();
}

void channel::check_and_call()
{
    if (!m_stack.empty()) {

        npdebug("Signal detected: ", label, " ", this)

        std::vector<std::weak_ptr<listener>> to_erase;

        // TODO, maybe it exists pop
        /* for each listener_s, catch signal */

        for (auto l : m_listeners)
        {
            listener::ptr pt;

            if (pt = l.lock())
            {
                for (const auto& sig : m_stack)
                    pt->invoke(sig);
            }
            else
                to_erase.push_back(l);
        }
    
        /* Erase invalidated listeners */
        m_listeners.erase(
            std::remove_if(
                m_listeners.begin(),
                m_listeners.end(),
                [](std::weak_ptr<listener> e) { return e.expired(); }
            )
        );

        m_stack.clear(); // TODO not so efficient
    }
}


/* signal class */

signal::signal(object *sender, 
               const std::string& id, 
               void *data, 
               priority_t p)

    :   labelled(id, true), 
        prioritized(p),
        m_sender(sender), 
        m_package(package(data))
{
}

bool signal::emit(const std::string& chan) const
{
    channel::ptr c = channel::find(chan);
    
    if (!c)
        return false;
    
    /* Finally emit in channel */
    c->emit(*this);

    return true;
}


/* listener_s class */

listener::listener(callback callback, const std::initializer_list<std::string>& filters)

    : m_callback(callback), m_filters(filters)
{

}

listener::~listener()
{
}

bool listener::check_in_filters(const std::string& filter) const
{
    for (const auto& f : m_filters)
    {
        if (filter == f)
            return true;
    }

    return false;
}

void listener::add_filter(const std::string& f)
{
    m_filters.push_back(f);
}

void listener::remove_filter(const std::string& f)
{
    m_filters.remove(f);
}

bool listener::connect(const std::string& chan)
{    
    channel::ptr c = channel::find(chan);

    if (!c)
        c->connect(this);

    return bool(c);
}

bool listener::disconnect(const std::string& chan)
{
    channel::ptr c = channel::find(chan);

    if (!c)
        c->disconnect(this);

    return bool(c);
}

void listener::invoke(const signal& sig)
{
    if ((!sig.label.empty() && check_in_filters(sig.label)) || 
        (sig.label.empty() && m_filters.empty()))
        m_callback(sig.m_sender, sig.m_package);
}

