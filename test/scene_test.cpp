#include "flatland/flatland.hpp"

#include "flatland/core/task.hpp"
#include "flatland/core/signal.hpp"

//#include "flatland/window.hpp"
#include "flatland/sprite.hpp"
#include "flatland/debug.hpp"

#include <wsdl2/video.hpp>
#include <wsdl2/event.hpp>


void keypressed(const wsdl2::event::key event) {
    if (event.type == wsdl2::event::key::action::down) {
        npdebug("you pressed ", static_cast<char>(event.code()));

        auto sprite = *flat::state::get()
            .current_scene()
            .sprites()
            .begin();

        if (event.code() == SDLK_ESCAPE) {
            flat::state::get().running = false;
        }

        if (event.code() == SDLK_a) {
            sprite->move({-5,  0});
        } else if (event.code() == SDLK_d) {
            sprite->move({ 5,  0});
        }

        if (event.code() == SDLK_w) {
            sprite->move({ 0, -5});
        } else if (event.code() == SDLK_s) {
            sprite->move({ 0,  5});
        }
    }
}

int main() {
    flat::initialize();

    flat::state& engine = flat::state::create("Scene Test");
    auto& win = engine.window();

    //auto render_task = engine.render.delegate_task(&flat::window::render, &win);

    auto keylist = engine.events.connect(&keypressed);
    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );

    auto s = engine.current_scene()
        .load_sprite("test/res/chiara.bmp");
        //.value();

    //win.insert(std::make_shared<flat::theater>());
    win.open();

    flat::run();
    flat::quit();

    return 0;
}
