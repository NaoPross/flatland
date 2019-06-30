#pragma once

#include "core/signal.hpp"
#include "core/task.hpp"

#include "scene.hpp"

#include <wsdl2/video.hpp>

#include <unordered_map>
#include <optional>
#include <memory>
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

        // singleton access methods
        static state& create(wsdl2::renderer& r);
        static state& get();

        /// renderer getter and setter
        void renderer(wsdl2::renderer& r);
        wsdl2::renderer& renderer();

        /// scenes
        scene& current_scene();
        void new_scene(); // pop and create a new scene 
        void push_scene(scene&& s);
        void pop_scene();

    private:
        explicit state(wsdl2::renderer& r);

        explicit state(state&& other) = delete;
        explicit state(const state& other) = delete;
        void operator=(const state& other) = delete;

        // scenes
        std::stack<scene> m_scenes;

        /// renderer object
        wsdl2::renderer *m_renderer;
        std::unordered_map<const std::string,
                           std::weak_ptr<wsdl2::texture>,
                           std::hash<std::string>> m_textures;
    };

    bool initialize();
    void quit();

    void run();
}
