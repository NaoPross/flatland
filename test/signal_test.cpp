#include "core/signal.hpp"
#include "core/task.hpp"
#include "debug.hpp"

#include <iostream>


using namespace flat::core;


void got_signal(int x, double y) {
    std::cout << "got signal with x=" <<  x << " and y=" << y << std::endl;
}


class test_emitter
{
private:
    channel& m_chan; 

public:
    test_emitter(channel& ch) : m_chan(ch) {}

    void send_str(const std::string& msg) {
        std::cout << "emitting signal with msg=" << msg << std::endl;
        m_chan.emit(signal(msg));
    }

    void send_num(int n) {
        std::cout << "emitting signal with n=" << n << std::endl;
        m_chan.emit(signal(n));
    }
};


class test_listener
{
private:
    template<typename T>
    using listener_of = typename std::shared_ptr<listener<T>>;

    listener_of<std::string> str_lis;
    listener_of<int> num_lis;

public:
    test_listener(channel& ch)
        : str_lis(ch.connect(&test_listener::got_string, this)),
          num_lis(ch.connect(&test_listener::got_number, this))
    {}

    void got_string(std::string msg) {
        std::cout << "got signal with msg=" << msg << std::endl;
    }

    void got_number(int n) {
        std::cout << "got signal with n=" << n << std::endl;
    }
};


int main() {
    // create a job to propagate signals
    job broadcaster;
    
    // create a channel
    channel chan(broadcaster);

    // test with a function
    auto fun_lis = chan.connect(got_signal);
    std::cout << "emitting signal with x=100, y=293.0" << std::endl;
    chan.emit(signal(100, 293.0));

    // test with a closure
    // TODO: fix
    // auto lambda_lis = chan.connect([](char a) {
    //     npdebug("got signal with a=", a);
    // });

    chan.emit(signal('f'));

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
