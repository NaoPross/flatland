#include "core/signal.hpp"
#include "core/task.hpp"
#include "debug.hpp"

#include <iostream>
#include <memory>


using namespace flat::core;

struct custom_type
{
    const char f;

    custom_type(char _f) : f(_f) {
        npdebug("instanciated custom_type");
    }

    custom_type(const custom_type& other) : f(other.f) {
        npdebug("copy constructed custom_type (bad)");
    }

    custom_type(custom_type&& other) : f(other.f) {
        npdebug("move constructed custom_type (efficient)");
    }

    ~custom_type() {
        npdebug("deleted custom_type instance");
    }
};


void got_signal(int x, double y) {
    std::cout << "got signal with x=" <<  x << " and y=" << y << std::endl;
}


class test_emitter
{
private:
    channel& m_chan; 

public:
    test_emitter(channel& ch) : m_chan(ch) {}

    void send_str(std::string&& msg) {
        std::cout << "emitting signal with msg=" << msg << std::endl;
        m_chan.emit(std::move(msg));
    }

    void send_num(int&& n) {
        std::cout << "emitting signal with n=" << n << std::endl;
        m_chan.emit(std::move(n));
    }

    void send_custom(custom_type&& c) {
        std::cout << "emitting signal with custom_type with f=" << c.f << std::endl;
        m_chan.emit(std::move(c));
    }

    void send_custom_ptr(custom_type&& c) {
        std::cout << "emitting signal with ptr to custom_type with f=" << c.f << std::endl;
        m_chan.emit(std::make_shared<custom_type>(std::move(c)));
    }
};


class test_listener
{
private:
    template<typename ...Ts>
    using listener_of = typename std::shared_ptr<listener<Ts...>>;

    listener_of<std::string> str_lis;
    listener_of<int> num_lis;
    listener_of<custom_type> cus_lis;
    listener_of<std::shared_ptr<custom_type>> ptr_lis;

public:
    test_listener(channel& ch)
        : str_lis(ch.connect(&test_listener::got_string, this)),
          num_lis(ch.connect(&test_listener::got_number, this)),
          cus_lis(ch.connect(&test_listener::got_custom, this)),
          ptr_lis(ch.connect(&test_listener::got_custom_ptr, this))
    {}

    void got_string(std::string msg) {
        std::cout << "got signal with msg=" << msg << std::endl;
    }

    void got_number(int n) {
        std::cout << "got signal with n=" << n << std::endl;
    }

    void got_custom(custom_type c) {
        std::cout << "got signal with custom_type with f=" << c.f << std::endl;
    }

    void got_custom_ptr(std::shared_ptr<custom_type> p) {
        std::cout << "got signal with ptr to custom_type with f=" << p->f << std::endl;
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
    chan.emit(100, 293.0);

    // test with a closure
    // TODO: fix
    // auto lambda_lis = chan.connect([](char a) {
    //     npdebug("got signal with a=", a);
    // });
    // chan.emit(signal('f'));

    // call job to propagate signals
    broadcaster();

    // test with members
    test_emitter e(chan);
    test_listener l(chan);

    e.send_str("hello world!");
    e.send_num(42);
    e.send_custom(custom_type('e'));
    e.send_custom_ptr(custom_type('x'));

    // call job to propagate signals
    broadcaster();

    return 0;
}
