#include "core/signal.hpp"
#include "core/task.hpp"
#include "debug.hpp"

#include <iostream>


using namespace flat::core;


void got_signal(int x, double y) {
    npdebug("got signal with x=", x, " and y=", y);
}


class test_emitter
{
private:
    channel& m_chan; 

public:
    test_emitter(channel& ch) : m_chan(ch) {}

    void send_str(const std::string& msg) {
        m_chan.emit(signal(msg));
        npdebug("emitted signal with msg=", msg);
    }

    void send_num(int n) {
        m_chan.emit(signal(n));
        npdebug("emitted signal with n=", n);
    }
};


class test_listener
{
private:
    template<typename T>
    using listener_of = typename std::shared_ptr<listener<T>>;

    listener_of<const std::string&> str_lis;
    listener_of<int> num_lis;

public:
    test_listener(channel& ch) {
        str_lis = ch.connect(&test_listener::got_string, this);
        num_lis = ch.connect(&test_listener::got_number, this);
    }

    void got_string(const std::string& msg) {
        npdebug("got signal with msg=", msg);
    }

    void got_number(int n) {
        npdebug("got signal with n=", n);
    }
};


int main() {
    // create a job to propagate signals
    job broadcaster;
    
    // create a channel
    channel chan(broadcaster);

    // test with a function
    chan.connect(got_signal);
    chan.emit(signal(100, 293.0));

    // call job to propagate signals
    broadcaster();

    // test with members
    test_emitter e(chan);
    test_listener l(chan);

    e.send_str("hello world!");
    e.send_num(42);

    // call job to propagate signals
    broadcaster();

    return 0;
}
