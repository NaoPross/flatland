#pragma once

#include "core/signal.hpp"
#include "core/task.hpp"
#include "wsdl2/wsdl2.hpp"

namespace flat {

    /* Structure containing the state of the engine i.e.
     * 
     *  - jobs (containing task) that are called when ran with flat::run
     *  - channels to broadcast events
     *  - global variables, like running
     *  - global graphics objects like textures
     * 
     */
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

        /// the event broadcast is handled by update
        state() : events(update) {}
    };


    bool initialize();
    void quit();

    void run(state& s);
}