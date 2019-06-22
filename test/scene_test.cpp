#include "flatland.hpp"

#include "wsdl2/event.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"

#include "window.hpp"
#include "sprite.hpp"
#include "debug.hpp"


void keypressed(const wsdl2::event::key event) {
    if (event.type == wsdl2::event::key::action::down) {
        npdebug("you pressed ", static_cast<char>(event.keysym.sym));

        auto sprite = *flat::state::get()
            .current_scene()
            .sprites()
            .begin();

        if (event.keysym.sym == SDLK_ESCAPE) {
            flat::state::get().running = false;
        }

        if (event.keysym.sym == SDLK_a) {
            sprite->move({-5,  0});
        } else if (event.keysym.sym == SDLK_d) {
            sprite->move({ 5,  0});
        }

        if (event.keysym.sym == SDLK_w) {
            sprite->move({ 0, -5});
        } else if (event.keysym.sym == SDLK_s) {
            sprite->move({ 0,  5});
        }
    }
}

int main() {
    flat::initialize();

    flat::state& engine = flat::state::get();
    flat::window win("Scene Test");

    auto render_task = engine.render.delegate_task(&flat::window::render, &win);

    auto keylist = engine.events.connect(&keypressed);
    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );

    engine.set_renderer(win.get_renderer());
    auto s = engine.current_scene()
        .load_sprite("test/res/chiara.bmp", mm::vec2<int>{0, 0})
        .value();

    win.insert(std::make_shared<flat::theater>());
    win.open();

    flat::run();
    flat::quit();

    return 0;
}
