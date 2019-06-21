#pragma once

#include "core/signal.hpp"
#include "core/task.hpp"

#include "scene.hpp"

#include "wsdl2/wsdl2.hpp"
#include "wsdl2/video.hpp"

#include <unordered_map>
#include <string>
#include <stack>


namespace flat
{

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
        friend class scene;

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

        scene& current_scene();

    private:
        state();
        
        // scenes
        std::stack<scene> m_scenes;

        /// renderer object
        wsdl2::renderer *m_renderer;
        std::unordered_map<std::string,
                           std::weak_ptr<wsdl2::texture>> m_textures;
    };


    bool initialize();
    void quit();

    void run();
}
