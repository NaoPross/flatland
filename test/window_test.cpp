#include "flatland/flatland.hpp"

#include "flatland/core/task.hpp"
#include "flatland/core/signal.hpp"

#include "flatland/debug.hpp"

#include <wsdl2/video.hpp>
#include <wsdl2/event.hpp>


void gloop() {

}

void key_cb(const wsdl2::event::key event) {
    if (event.type == wsdl2::event::key::action::down) {
        npdebug("you pressed ", static_cast<char>(event.code()));
        if (event.code() == SDLK_ESCAPE) {
            flat::state::get().running = false;
        }
    }
}

int main() {

    flat::initialize();

    flat::state& engine = flat::state::create("Window Test");

    engine.update.add_task(&gloop);

    //auto render_task = engine.render.delegate_task(&flat::window::render, &win);

    auto keylist = engine.events.connect(&key_cb);
    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );

    auto& win = engine.window();

    win.open();

    flat::run();
    flat::quit();

    return 0;
}

