#include "core/signal.hpp"
#include "core/task.hpp"
#include "object.hpp"
#include "window.hpp"
#include "flatland.hpp"
#include "exceptions/forcequit.hpp"

#include <iostream>

using namespace std;
using namespace flat;
using namespace flat::core;


class sender : public object
{
    const char * message;
    channel::ptr chan; 

public:

    sender(const char * message, channel::ptr chan) : message(message), chan(chan)
    {
    }

    void send()
    {
        signal msg(this, "", (void*)message);
        chan->emit(msg);
    }
};

void function_listener(const object*, core::signal::package msg)
{
    cout << "Funzione: " << msg.get<const char>() << endl;
}

class c_listener
{

    listener::ptr lis;

public:

    c_listener(channel::ptr chan)
    {
        lis = chan->connect(&c_listener::method_listener, *this);
    }

    void method_listener(const object *o, signal::package msg)
    {
        cout << "Metodo" << msg.get<const char>() << endl;
    }
};

/* Objects definition */

channel::ptr alpha;
sender * m_sender;
c_listener * m_listener;
listener::ptr f_listener;

int steps = 0;

void lifeloop()
{
    if (!(steps % 10))
        cout << "Step: " << steps << endl;

    if (!(steps % 40))
        m_sender->send();

    if (++steps > 2000)
    {
        signal quit(0, "quit");

        // quit request
        flat::core_channel().emit(quit);
    }

    if (steps > 2100)
        throw flat::ForceQuit("Too many steps");
}

int main()
{
    FlatWindow win(600, 900, "Test 3");
    flat_status status;

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

    delete m_sender;
    delete m_listener;

    return 0;
}
