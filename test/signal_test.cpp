#include "core/signal.hpp"
#include "core/task.hpp"
#include "object.hpp"
#include "window.hpp"
#include "flatland.hpp"
#include "exceptions/forcequit.hpp"

#include "debug.hpp"

using namespace std;
using namespace flat;
using namespace flat::core;


class sender : public object
{
    const char *m_message;
    channel::ptr m_chan; 

public:

    sender(const char * message, channel::ptr chan) : m_message(message), m_chan(chan)
    {
    }

    void send()
    {
        signal<const char*> msg(m_message);
        m_chan->emit(m_msg);
    }
};

void function_listener(const char *msg)
{
    cout << "Funzione: " << msg << endl;
}

class c_listener
{
    std::shared_ptr<listener<const char*>> lis;

public:

    c_listener(channel::ptr chan)
    {
        m_lis = chan->connect(&c_listener::method_listener, this);
    }

    void method_listener(const char *msg)
    {
        cout << "Metodo: " << msg << endl;
    }
};

/* Objects definition */

channel::ptr alpha;
sender * m_sender;
c_listener * m_listener;
std::shared_ptr<listener<const char*>> f_listener;

int steps = 0;

void lifeloop()
{
    if (!(steps % 10))
        cout << "Step: " << steps << endl;

    if (!(steps % 40))
        m_sender->send();

    if (++steps > 200)
    {
        signal<const char*> quit("quit");

        // quit request
        flat::core_channel().emit(quit);
    }

    if (steps > 205)
        throw flat::ForceQuit("Too many steps");
}

int main()
{
    FlatWindow win(600, 900, "Test 3");
    flat_status status;

    npdebug("Initializing channel alpha")

    alpha = channel::create("alpha");

    if (alpha == nullptr)
    {
        cout << "Could not create channel alpha" << endl;
        return -1;
    }

    // create sender
    m_sender = new sender("Ciao", alpha);
    m_listener = new c_listener(alpha);

    // Connect listener to alpha channel
    f_listener = alpha->connect(&function_listener);

    // bind counter task
    task::ptr looptask = flat::main_job().delegate_task(lifeloop);

    init_flatland(&win, status, 60);

    npdebug("Deleting m_sender")
    delete m_sender;

    npdebug("Deleting m_listener")
    delete m_listener;

    alpha = nullptr; // out of scope
    f_listener = nullptr;

    npdebug("alpha use count: ", alpha.use_count())
    npdebug("f_listener use count: ", f_listener.use_count())

    npdebug("Exiting")

    return 0;
}
