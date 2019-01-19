#ifndef __FLAT_SIGNAL_H__
#define __FLAT_SIGNAL_H__

#include <map>
#include <list>
#include <set>
#include <initializer_list>
#include "flatobject.h"
#include "types.h"

/* Post-process signal delivery when it's emitted */
#define FSIG_PRIOR_INSTANT 0

/* Process and emitted signal instantaneously */
#define FSIG_PRIOR_DEFAULT 5

class FlatSignal : virtual public FlatObject
{

public:

    FlatObject * sender;
    void * data;
    Uint8 priority;

    FlatSignal(FlatObject * sender, void *data = 0, Uint8 priority = 5);

    /* Alias to SignalChannel::emit() */
    bool emit(const std::string& channel) const;
};

class FlatListener;

/* Functor for order comparison */
struct sig_prior_cmp {
    
    bool operator()(const FlatSignal&, const FlatSignal&) const;
};

typedef std::set<FlatSignal, sig_prior_cmp> SignalStack;

/* Channel class */
class SignalChannel : virtual public FlatObject
{
    /* Post processing signal stacking */
    SignalStack stack;

    /* Listeners list */
    std::list<FlatListener*> listeners;

    /* Synchronous task checking for signals */
    task_s * checker;

    /* Channel mapping */
    static std::map<std::string, SignalChannel*> channels;    
 
public:

    SignalChannel(const std::string& id = "");
    ~SignalChannel();

    void emit(const FlatSignal&);

    void connect(FlatListener*);
    void disconnect(FlatListener*);
   
    static SignalChannel * findChannel(const std::string&); 

    void post_processing(void*);
};

/* Listener class */
class FlatListener : virtual public FlatObject
{
    std::list<std::string> filters;

    bool checkInFilters(const std::string&) const;

protected:

    virtual void callback(FlatObject *sender, void *) = 0;

public:

    FlatListener(const std::initializer_list<std::string>& filters = {});
    virtual ~FlatListener() {}

    void execute(const FlatSignal&);

    void addFilter(const std::string&);
    void removeFilter(const std::string&);

    bool connect(const std::string&);
    bool disconnect(const std::string&);
};

#endif
