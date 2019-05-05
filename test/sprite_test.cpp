#include "flatland.hpp"

#include "wsdl2/event.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"

#include "window.hpp"
#include "sprite.hpp"
#include "debug.hpp"


void gloop() {

}

void key_cb(const wsdl2::event::key event) {
    if (event.type == wsdl2::event::key::action::down) {
        npdebug("you pressed ", static_cast<char>(event.keysym.sym));
        if (event.keysym.sym == SDLK_ESCAPE) {
            flat::state::get().running = false;
        }
    }
}

int main() {
    flat::initialize();

    flat::state& engine = flat::state::get();
    flat::window win("Sprite Test");

    engine.update.add_task(&gloop);

    auto render_task = engine.render.delegate_task(&flat::window::render, &win);

    auto keylist = engine.events.connect(&key_cb);
    auto quitlist = engine.events.connect<void, wsdl2::event::quit>(
        [&](wsdl2::event::quit e) {
            engine.running = false;
        }
    );


    // sprite initialization
    if (auto surf = wsdl2::surface::load("test/res/chiara.bmp")) {
        auto tex = wsdl2::texture(win.get_renderer(), *surf);
        auto tileset = std::make_shared<flat::tileset>(std::move(tex));
        auto sprite = std::make_shared<flat::sprite>(mm::vec2<int>{0, 0}, tileset);

        sprite->move(mm::vec2<int>{100, 100});

        win.insert(std::static_pointer_cast<flat::renderable>(sprite));
    } else {
        npdebug("failed to load texture")
        return -1;
    }
    
    win.open();

    flat::run();
    flat::quit();

    return 0;
}
