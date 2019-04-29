#include "flatland.hpp"

#include "window.hpp"
#include "debug.hpp"

#include "wsdl2/event.hpp"

#include <functional>
#include <iostream>


namespace test {
    struct state {
        bool game_over = false;
    };

    void update_model() {

    }
}


int main(int, char**) {
    flat::initialize();

    flat::state engine;
    flat::window win("Flatland Test");

    // connect events callbacks
    auto keylist = engine.events.connect<void, wsdl2::event::key>(
        [&](wsdl2::event::key event) {
            if (event.type == wsdl2::event::key::action::down) {
                if (event.keysym.sym == SDLK_ESCAPE) {
                    engine.running = false;
                }
            }
        }
    );

    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );

    // add task to update the game data model
    engine.update.add_task(test::update_model);

    // add a rendering task
    auto render_task = engine.render.delegate_task(&flat::window::render, &win);
    win.open();

    flat::run(engine);
    flat::quit();

    return 0;
}
