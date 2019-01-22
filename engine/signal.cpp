#include "signal.hpp"
#include "task.hpp"

using namespace std;
using namespace flat::core;

bool channel::sig_prior_cmp::operator()(const signal& s, const signal& g) const
{
    return s.prior <= g.prior;
}

map<string, channel*> channel::channels;    

channel::channel(const string& id, task::priority prior)
{
    channel * other = find_channel(id);

    if (!other)
        //TODO throw exception
        ;

    /* Initialize task, post-process, fifth priority */
    checker = new task<channel>(  this, 
                                  &channel::post_processing, 
                                  0, 
                                  prior);

    string ID = (id.empty()) ? object::random_id() : id;

    set_id(ID);

    channels.insert(pair<string, channel*>(ID, this));
}

channel::~channel()
{
    channels.erase(get_id());

    /* Finalize task */
    delete checker;
}

void channel::emit(const signal& sig)
{
    if (!sig.priority) {

        /* Execute immediately */
        for (auto lis : listeners)
            lis->execute(sig);

    } else
        /* Insert in order of priority */
        stack.insert(sig);
}

void channel::connect(listener* l)
{
    /* Control not to double */
    for (auto lis : listeners)
    {
        if (lis == l)
            return;
    }

    listeners.push_back(l);
}

void channel::disconnect(listener* l)
{
    listeners.remove(l);
}

   
channel * channel::find_channel(const string& id)
{
    if (id.empty())
        return 0;

    auto it = channels.find(id);

    return (it == channels.end()) ? 0 : (*it).second;
}

void channel::post_processing(void*)
{
    if (!stack.empty()) {

        /* for each listener_s, catch signal */
        for (const auto& signal : stack)
        {
            for (auto l : listeners)
                l->execute(signal);
        }

        stack.clear();
    }
}



/* signal class */

signal::signal(flat::core::object *sender, const string& id, void *data, Uint8 priority)
    : sender(sender), data(data), priority(priority)
{
    set_id(id);
}

bool signal::emit(const string& channel) const
{
    channel * chan = channel::find_channel(channel);
    
    if (!chan)
        return false;
    
    /* Finally emit in channel */
    chan->emit(*this);

    return true;
}



/* listener_s class */

listener::listener(   std::function<void()> m_callback,
                      const std::initializer_list<std::string>& filters)

    : m_callback(m_callback), filters(filters)
{

}

listener::~listener()
{
    if (parent != 0)
        parent->disconnect(this);
}

bool listener::check_in_filters(const std::string& filter) const
{
    for (const auto& f : filters)
    {
        if (filter == f)
            return true;
    }

    return false;
}

void listener::add_filter(const std::string& f)
{
    filters.push_back(f);
}

void listener::remove_filter(const std::string& f)
{
    filters.remove(f);
}

bool listener::connect(const std::string& chan)
{    
    channel * c = channel::find_channel(chan);

    if (!c)
        c->connect(this);

    parent = c;

    return c != 0;
}

bool listener_s::disconnect(const std::string& chan)
{
    channel * c = channel::find_channel(chan);

    if (!c)
        c->connect(this);

    parent = 0;

    return c != 0;
}

void listener::invoke(const flat::core::signal&)
{
    if ((!sig.get_id().empty() && check_in_filters(sig.get_id())) || 
    (sig.get_id().empty() && filters.empty()))
        m_callback(sig.sender);
}

