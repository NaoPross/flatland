#include "core/task.hpp"

#include <functional>
#include <iostream>


// test class
struct message {
    std::string motd = "today we have no motd!";
    std::string date = "1 Jan 1970";

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

    using namespace flat::core;


    std::cout << "Testing functions" << std::endl;
    std::cout << "should print once: hello!" << std::endl;

    job f_job;

    // test a function
    auto ciao_fn_task = f_job.make_task(hello);

    // test a function ad make the pointer go out of scope
    {
        auto hello_fn_task = f_job.make_task(ciao);
    }

    f_job.invoke_tasks();

    std::cout << std::endl;
    std::cout << "Testing methods" << std::endl;
    std::cout << "  should print once: today we have no motd!" << std::endl;
    job m_job;

    // test a method
    message m;
    m_job.make_task(std::bind(&message::print_motd, m));

    // test a method of an object that goes out of scope
    {
        message out;
        m_job.make_task(std::bind(&message::print_date, out));
    }

    // invoke tasks
    // should print:
    //
    //  hello!
    //  hello world!
    m_job.invoke_tasks();

    return 0;
}
