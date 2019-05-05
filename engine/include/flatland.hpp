#pragma once

#include "core/signal.hpp"
#include "core/task.hpp"

#include "renderable.hpp"
#include "sprite.hpp"

#include "wsdl2/wsdl2.hpp"
#include "wsdl2/video.hpp"

#include <unordered_map>


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

        /// singleton pattern
        state(const state& other) = delete;
        void operator=(const state& other) = delete;

        static state& get();

        void set_renderer(wsdl2::renderer& r);
        wsdl2::renderer& renderer();

    private:
        /// the event broadcast is handled by update
        state() : events(update) {}

        /// renderer object
        wsdl2::renderer *m_renderer;

        rendergroup m_renderables;
        std::unordered_map<std::string, std::weak_ptr<tileset>> m_textures;
    };


    bool initialize();
    void quit();

    void run();
}