#include "flatland/flatland.hpp"

#include "flatland/core/task.hpp"
#include "flatland/core/signal.hpp"

#include "flatland/sprite.hpp"
#include "flatland/debug.hpp"

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

    flat::state& engine = flat::state::create("Sprite Test");

    engine.update.add_task(&gloop);

    //auto render_task = engine.render.delegate_task(&flat::window::render, &win);

    auto keylist = engine.events.connect(&key_cb);
    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );

    auto& win = engine.window();

    //engine.new_scene();

    // there must be a start up scene
    auto sc = engine.current_scene();

    // sprite initialization
    if (auto surf = wsdl2::surface::load("test/res/chiara.bmp")) {
        auto tex = std::make_shared<wsdl2::static_texture>(win.get_renderer(), *surf);
        auto tileset = std::make_shared<flat::tileset>(tex);

        sc.load_sprite(tileset);
        //auto sprite = std::make_shared<flat::sprite>(tileset);
        //win.insert(std::static_pointer_cast<flat::trait::renderable>(sprite));
    } else {
        npdebug("failed to load texture")
        return -1;
    }

    win.open();
    flat::run();
    flat::quit();

    return 0;
}
