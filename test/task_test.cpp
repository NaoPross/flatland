#include "task.hpp"

#include <functional>
#include <iostream>


struct hello_class {
    const std::string motd = "hello world!";

    void print_motd() {
        std::cout << motd << std::endl;
    }
};


void hello() {
    std::cout << "hello!" << std::endl;
}

int main(int argc, char *argv[]) {

    using namespace flat::core;

    job job;

    // test a function
    task t_func(hello);
    job.add_task(t_func);

    // test a member function
    hello_class h;
    task t_mem([&](){ h.print_motd(); });
    job.add_task(t_mem);

    // invoke tasks
    job.invoke_tasks();

    return 0;
}
