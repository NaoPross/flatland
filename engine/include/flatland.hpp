#pragma once

#include "core/signal.hpp"
#include "core/task.hpp"
#include "wsdl2/wsdl2.hpp"

namespace flat {

    /* Singleton data structure containing the state of the engine i.e.
     * 
     *  - jobs (containing task) that are called when ran with flat::run
     *  - channels to broadcast events
     *  - global variables, like running
     *  - global graphics objects like textures
     * 
     */
    struct state {
    public:
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

        /// signleton pattern
        state(const state& other) = delete;
        void operator=(const state& other) = delete;

        static state& get();

    private:
        /// the event broadcast is handled by update
        state() : events(update) {}
    };


    bool initialize();
    void quit();

    void run();
}