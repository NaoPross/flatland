#include "flatland/flatland.hpp"

#include "flatland/core/task.hpp"
#include "flatland/core/signal.hpp"

//#include "flatland/window.hpp"
#include "flatland/sprite.hpp"
#include "flatland/debug.hpp"

#include <wsdl2/video.hpp>
#include <wsdl2/event.hpp>

flat::trait::proj::pixel_positioner * ball_pos;

void keypressed(const wsdl2::event::key event) {
    if (event.type == wsdl2::event::key::action::down) {
        npdebug("you pressed ", static_cast<char>(event.code()));

        auto sprite = ball_pos;

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

    {

    /*
     * Background image:
     *  - Fullscreen projector
     *  - Positioning index: 0
     */
    auto background = engine.current_scene().load_sprite("res/static_background.jpg", new flat::trait::proj::fullscreen(win));
    background->z = 0;

    /*
     * Ball image:
     *  - Positioner in pixel format projector
     *  - Positioning index: 1
     */
    auto ball = engine.current_scene().load_sprite("res/baseball_ball.png", ball_pos = new flat::trait::proj::pixel_positioner({200, 200}, {150, 150}));
    ball->z = 1;

    if (background == nullptr) {
        npdebug("Background is null")
    }

    if (ball == nullptr) {
        npdebug("Ball is null")
    }
        //.value();

    //win.insert(std::make_shared<flat::theater>());
    win.open();

    flat::run();
    }

    return 0;
}
