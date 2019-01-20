#include "flatsignal.h"
#include "flattask.h"

using namespace std;

bool sig_prior_cmp::operator()(const FlatSignal& s, const FlatSignal& g) const
{
    return s.priority <= g.priority;
}

map<string, SignalChannel*> SignalChannel::channels;    

SignalChannel::SignalChannel(const string& id)
{
    SignalChannel * other = findChannel(id);

    if (!other)
        //TODO throw exception
        ;

    /* Initialize task, post-process, fifth priority */
    checker = new FlatTask<SignalChannel>(this, &SignalChannel::post_processing, 0, false, 4);

    string ID = (id.empty()) ? FlatObject::randomID() : id;

    setID(ID);

    channels.insert(pair<string, SignalChannel*>(ID, this));
}

SignalChannel::~SignalChannel()
{
    channels.erase(getID());

    /* Finalize task */
    delete checker;
}

void SignalChannel::emit(const FlatSignal& sig)
{
    if (!sig.priority) {

        /* Execute immediately */
        for (auto listener : listeners)
            listener->execute(sig);

    } else
        /* Insert in order of priority */
        stack.insert(sig);
}

void SignalChannel::connect(FlatListener* l)
{
    /* Control not to double */
    for (auto listener : listeners)
    {
        if (listener == l)
            return;
    }

    listeners.push_back(l);
}

void SignalChannel::disconnect(FlatListener* l)
{
    listeners.remove(l);
}

   
SignalChannel * SignalChannel::findChannel(const string& id)
{
    if (id.empty())
        return 0;

    auto it = channels.find(id);

    return (it == channels.end()) ? 0 : (*it).second;
}

void SignalChannel::post_processing(void*)
{
    if (!stack.empty()) {

        /* for each listener, catch signal */
        for (const auto& signal : stack)
        {
            for (auto listener : listeners)
                listener->execute(signal);
        }

        stack.clear();
    }
}



/* FlatSignal class */

FlatSignal::FlatSignal(FlatObject *sender, const string& id, void *data, Uint8 priority)
    : sender(sender), data(data), priority(priority)
{
    setID(id);
}

bool FlatSignal::emit(const string& channel) const
{
    SignalChannel * chan = SignalChannel::findChannel(channel);
    
    if (!chan)
        return false;
    
    /* Finally emit in channel */
    chan->emit(*this);

    return true;
}



/* FlatListener class */

FlatListener::FlatListener(const initializer_list<string>& filters) 
    : filters(filters) {}

bool FlatListener::checkInFilters(const std::string& filter) const
{
    for (const auto& f : filters)
    {
        if (filter == f)
            return true;
    }

    return false;
}

void FlatListener::execute(const FlatSignal& sig)
{
    if ((!sig.getID().empty() && checkInFilters(sig.getID())) || 
        (sig.getID().empty() && filters.empty()))
        callback(sig.sender, sig.data);
}

void FlatListener::addFilter(const std::string& f)
{
    filters.push_back(f);
}

void FlatListener::removeFilter(const std::string& f)
{
    filters.remove(f);
}

bool FlatListener::connect(const std::string& chan)
{    
    SignalChannel * c = SignalChannel::findChannel(chan);

    if (!c)
        c->connect(this);

    return c != 0;
}

bool FlatListener::disconnect(const std::string& chan)
{
    SignalChannel * c = SignalChannel::findChannel(chan);

    if (!c)
        c->connect(this);

    return c != 0;
}

