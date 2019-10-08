#include "flatland/flatland.hpp"

#include "flatland/debug.hpp"

#include <wsdl2/event.hpp>

#include <functional>
#include <iostream>

#include "flatland/scene.hpp"

namespace test {

class scene
{
public:

    struct positioner : public flat::trait::projector
    {
        positioner( mm::vec2<double> _pos,
                    mm::vec2<double> _size,
                    mm::vec2<double>& _expand,
                    mm::vec2<double>& _centre)
            : pos(_pos), size(_size), expand(_expand), centre(_centre)
        {}

        mm::vec2<double> pos;
        mm::vec2<double> size;

        mm::vec2<double>& expand; 
        mm::vec2<double>& centre; 

        virtual wsdl2::rect projection() const override {
            return wsdl2::rect{(int)((pos[0] + centre[0]) / expand[0] - size[0]/2.0), (int)((pos[1] + centre[1]) / expand[1]  - size[1]/2.0), (int)size[0], (int)size[1]};
        }
    };

    scene(flat::scene& _scene, mm::vec2<double> _expand, mm::vec2<double> _centre) 
        : m_scene(_scene), expand(_expand), centre(_centre) {}

    inline std::pair<std::shared_ptr<flat::sprite>, positioner*>
    load_sprite(const std::string& path, mm::vec2<double> pos, mm::vec2<double> size, unsigned index = 0)
    {
        positioner * proj = new positioner(pos, size, expand, centre);
        return {m_scene.load_sprite(path, proj, index), proj};
    }

private:

    flat::scene& m_scene;

    // transformation core
    mm::vec2<double> expand; 
    mm::vec2<double> centre; 
};


    struct state {
        bool game_over = false;
    };

    void update_model() {

    }

    void keypressed(scene::positioner *ball_pos, const wsdl2::event::key event) {
        if (event.type == wsdl2::event::key::action::down) {
            //npdebug("You pressed ", static_cast<char>(event.code()));
 
            if (event.code() == SDLK_ESCAPE) {
                flat::state::get().running = false;
            }
 
            if (event.code() == SDLK_a) {
                ball_pos->pos += mm::vec2<double>({-0.04,  0});
            } else if (event.code() == SDLK_d) {
                ball_pos->pos += mm::vec2<double>({0.04,  0});
            }
 
            if (event.code() == SDLK_w) {
                ball_pos->pos += mm::vec2<double>({0,  0.04});
            } else if (event.code() == SDLK_s) {
                ball_pos->pos += mm::vec2<double>({0,  -0.04});
            }
        }
    }
}


int main(int, char**) {
    flat::initialize();

    flat::state& engine = flat::state::create("Flatland Test");
    auto& win = engine.window();

    // connect events callbacks
    auto keylist = engine.events.connect<void, wsdl2::event::key>(
        [&](wsdl2::event::key event) {
            if (event.type == wsdl2::event::key::action::down) {
                if (event.code() == SDLK_ESCAPE) {
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

    wsdl2::point win_size = win.size();    
    flat::scene& current = engine.current_scene();

    test::scene game_scene(current, {2.0/win_size.x, -2.0/win_size.y}, {1, -1});

    /*
     * Background image:
     *  - Fullscreen projector
     *  - Positioning index: 0
     */
    auto background = current.load_sprite("res/static_background.jpg", new flat::trait::proj::fullscreen(win));
    background->z = 0;

    /*
     * Ball image:
     *  - Positioner in pixel format projector
     *  - Positioning index: 1
     */
    auto ball = game_scene.load_sprite("res/baseball_ball.png", {0, 0}, {150, 150});
    test::scene::positioner * projector = ball.second;
    ball.first->z = 1;

    if (background == nullptr) {
        npdebug("Background is null")
    }

    if (ball.first == nullptr) {
        npdebug("Ball is null")
    }

    auto keyupdate = engine.events.connect<void, wsdl2::event::key>(
            [projector](const wsdl2::event::key event){
                test::keypressed(projector, event);
            });

    // add a rendering task
    //auto render_task = engine.render.delegate_task(&flat::window::render, &win);
    win.open();

    flat::run();

    return 0;
}
