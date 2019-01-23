#include "core/task.hpp"

#include <functional>
#include <iostream>

using namespace flat::core;

// test class
class message {
private:
    task::ptr mytask;

    std::string motd = "today we have no motd!";
    std::string date = "1 Jan 1970";

public:
    message(job& job) {
        // add an example job
        mytask = job.delegate_task(&message::print_motd, *this);
    }

    void print_date() {
        std::cout << date << std::endl;
    }

    void print_motd() {
        std::cout << motd << std::endl;
    }
};

// test functions
void hello() {
    std::cout << "hello!" << std::endl;
}

void ciao() {
    std::cout << "ciao!" << std::endl;
}


int main(int argc, char *argv[]) {


    std::cout << "Testing functions" << std::endl;
    std::cout << "should print once: hello!" << std::endl;

    job f_job;

    // test a function
    auto hello_fn_task = f_job.delegate_task(hello);

    // test a function ad make the pointer go out of scope
    {
        auto ciao_fn_task = f_job.delegate_task(ciao);
    }

    f_job();

    std::cout << std::endl;
    std::cout << "Testing methods" << std::endl;
    std::cout << "should print once: today we have no motd!" << std::endl;

    job m_job;

    // test a method
    message m(m_job);

    // test a method of an object that goes out of scope
    {
        message out(m_job);
    }

    // invoke tasks
    // should print:
    //
    //  hello!
    //  hello world!
    m_job();

    return 0;
}
