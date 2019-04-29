#include "flatland.hpp"

#include "wsdl2/event.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"

#include "window.hpp"
#include "debug.hpp"


void gloop()
{

}

void key_cb(const wsdl2::event::key event)
{
    if (event.type == wsdl2::event::key::action::down) {
        npdebug("you pressed ", static_cast<char>(event.keysym.sym));
    }
}

int main() {

    flat::initialize();

    flat::state engine;
    flat::window win("Window Test");

    engine.update.add_task(&gloop);

    auto render_task = engine.render.delegate_task(&flat::window::render, &win);

    auto keylist = engine.events.connect(&key_cb);
    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );

    win.open();
    flat::run(engine);
    flat::quit();

    return 0;
}

