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
        for (auto listener_s : listener_ss)
            listener_s->execute(sig);

    } else
        /* Insert in order of priority */
        stack.insert(sig);
}

void channel::connect(listener_s* l)
{
    /* Control not to double */
    for (auto listener_s : listener_ss)
    {
        if (listener_s == l)
            return;
    }

    listener_ss.push_back(l);
}

void channel::disconnect(listener_s* l)
{
    listener_ss.remove(l);
}

   
channel * channel::findChannel(const string& id)
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
            for (auto listener_s : listener_ss)
                listener_s->execute(signal);
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

listener_s::listener_s(const initializer_list<string>& filters) 
    : filters(filters) {}

bool listener_s::check_in_filters(const std::string& filter) const
{
    for (const auto& f : filters)
    {
        if (filter == f)
            return true;
    }

    return false;
}

void listener_s::add_filter(const std::string& f)
{
    filters.push_back(f);
}

void listener_s::remove_filter(const std::string& f)
{
    filters.remove(f);
}

bool listener_s::connect(const std::string& chan)
{    
    channel * c = channel::find_channel(chan);

    if (!c)
        c->connect(this);

    return c != 0;
}

bool listener_s::disconnect(const std::string& chan)
{
    channel * c = channel::find_channel(chan);

    if (!c)
        c->connect(this);

    return c != 0;
}

