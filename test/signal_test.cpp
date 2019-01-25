#include "core/signal.hpp"
#include "core/task.hpp"
#include "object.h"
#include "flatland.hpp"
#include <iostream>

using namespace std;
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

void function_listener(const object*, signal::package msg)
{
    cout << "Funzione: " << msg.get<const char>() << endl;
};

class class_listener
{

    listener::ptr lis;

public:

    class_listener(channel::ptr chan)
    {
        lis = chan->connect(&class_listener::listen, *this);
    }

    void listen(const object*, signal::package)
    {
        cout << "Metodo" << msg.get<const char>() << endl;
    }
};

/* Objects definition */

channel::ptr alpha;
sender * m_sender;
class_listener * m_listener;
listener::ptr f_listener;

int count = 0;

void lifeloop()
{
    if (!(count % 10))
        cout << "Step: " << count << endl;

    if (!(count % 40))
        m_sender.send();

    if (++count > 2000)
    {
        signal quit(0, "quit");

        // quit request
        flat::core_channel().emit(quit);
    }
}

int main()
{
    FlatWindow win(600, 900, "Test 3");
    flat_status status;

    alpha = channel::create("alpha");

    // create sender
    m_sender = new sender("Ciao", alpha);
    m_listener = new class_listener(alpha);

    // Connect listener to alpha channel
    f_listener = alpha.connect(&function_listener);

    // bind counter task
    task::ptr looptask = flat::main_job().delegate_task(&lifeloop);

    init_flatland(&win, status, 60);

    delete m_sender;
    delete m_listener;

    return 0;
}
