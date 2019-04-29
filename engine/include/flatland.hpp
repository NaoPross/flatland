#pragma once

#include <chrono>
#include <thread>

#include "core/signal.hpp"
#include "core/task.hpp"
#include "wsdl2/wsdl2.hpp"

namespace flat {
    struct state {
        /// indicate whether the engine is running
        bool running = false;

        /// tasks to update the game
        core::job update;
        const unsigned ups = 60;

        /// tasks to render the game
        core::job render;
        const unsigned fps = 60;

        /// input events
        core::channel events;

        state() : events(update) {}
    };


    bool initialize();
    void quit();

    void run(state& s);
}