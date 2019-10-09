#include "flatland/flatland.hpp"

#include "flatland/debug.hpp"

#include <wsdl2/event.hpp>

#include <functional>
#include <iostream>

#include "flatland/scene.hpp"

class test
{
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
            return wsdl2::rect{(int)((pos[0] + centre[0] - size[0]/2.0) / expand[0]), (int)((pos[1] + centre[1] - size[1]/2.0) / expand[1]), (int)(size[0] / expand[0]), (int)(-size[1] / expand[1])};
            //npdebug(out.x, " ", out.y, " ", out.w, " ", out.h)
            //return out;
        }
    };

public:

    test(flat::state& engine, const mm::vec2<double>& bounds_x, const mm::vec2<double>& bounds_y) 
        : m_state(engine)
    {
        auto& scene = engine.current_scene();
        auto& win = engine.window();

        set_space(bounds_x, bounds_y);

        // determine projection
        eval_space();

        // connect events callbacks
        quit_keylist = m_state.events.connect(&test::escape_quit, this);
        move_keylist = m_state.events.connect(&test::keypressed, this);

        quitlist = m_state.events.connect(&test::close_quit, this);

        update_task = m_state.update.delegate_task(&test::update_model, this);

        /*
         * Background image:
         *  - Fullscreen projector
         *  - Positioning index: 0
         */
        background = scene.load_sprite("res/static_background.jpg", new flat::trait::proj::fullscreen(win));
        background->z = 0;

        /*
         * Ball image:
         *  - Positioner in pixel format projector
         *  - Positioning index: 1
         */
        ball_projector = new positioner({0, 0}, {0.2, 0.2}, expand, centre);
        ball = scene.load_sprite("res/baseball_ball.png", ball_projector);
        ball->z = 1;

        if (background == nullptr) {
            npdebug("Background is null")
        }

        if (ball == nullptr) {
            npdebug("Ball is null")
        }

        // open the window
        win.open();
    }

    void set_space(const mm::vec2<double>& bounds_x, const mm::vec2<double>& bounds_y)
    {
        space_size = {abs(bounds_x[1] - bounds_x[0]), abs(bounds_y[1] - bounds_y[0])};
        centre = {-bounds_x[0], bounds_y[0]};
        npdebug(space_size)
        npdebug(centre)
        eval_space();
    }

    void eval_space()
    {
        auto win_size = m_state.window().size();
        expand = {space_size[0]/win_size.x, -space_size[1]/win_size.y};
    }

    void escape_quit(wsdl2::event::key event) {
        if (event.type == wsdl2::event::key::action::down && 
            event.code() == SDLK_ESCAPE)
                m_state.quit();
    }

    void close_quit(wsdl2::event::quit e) {
        m_state.quit();
    }

    void update_model() {

    }

    void keypressed(const wsdl2::event::key event) {
        if (event.type == wsdl2::event::key::action::down) {
            //npdebug("You pressed ", static_cast<char>(event.code()));
 
            if (event.code() == SDLK_ESCAPE) {
                flat::state::get().running = false;
            }
 
            if (event.code() == SDLK_a) {
                ball_projector->pos += mm::vec2<double>({-0.04,  0});
            } else if (event.code() == SDLK_d) {
                ball_projector->pos += mm::vec2<double>({0.04,  0});
            }
 
            if (event.code() == SDLK_w) {
                ball_projector->pos += mm::vec2<double>({0,  0.04});
            } else if (event.code() == SDLK_s) {
                ball_projector->pos += mm::vec2<double>({0,  -0.04});
            }
        }
    }

private:
    // flatland state 
    flat::state& m_state;

    // transformation relative indication
    mm::vec2<double> space_size; 

    // transformation core
    mm::vec2<double> expand; 
    mm::vec2<double> centre; 

    // listeners
    std::shared_ptr<flat::core::listener<wsdl2::event::key>> quit_keylist;
    std::shared_ptr<flat::core::listener<wsdl2::event::key>> move_keylist;

    std::shared_ptr<flat::core::listener<wsdl2::event::quit>> quitlist;

    // update task
    std::shared_ptr<flat::core::task> update_task;

    // sprites
    std::shared_ptr<flat::sprite> background;
    std::shared_ptr<flat::sprite> ball;

    // positioner of the ball
    positioner *ball_projector;
};



int main(int, char**) {

    flat::initialize();
    flat::state& engine = flat::state::create("Flatland Test");

    test game_scene(engine, {-1.0, 1.0}, {-1.0, 1.0});

    flat::run();

    return 0;
}
