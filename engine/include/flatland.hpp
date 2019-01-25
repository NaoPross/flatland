#pragma once

#include <functional>

namespace flat {
    namespace core {
        class job;
        class task;
        class channel;
    }

    void intialize(std::function<void()> loop);
    void run(unsigned framerate);
    void quit();

    /* Engine channels */
    // TODO:
    // core::channel& core_chan();
    // core::channel& error_chan();

    /* Main job access */
    core::job& gamejob();

}
