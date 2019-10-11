#include "flatland/flatland.hpp"

#include "flatland/debug.hpp"

#include <wsdl2/event.hpp>

#include <functional>
#include <iostream>

#include "flatland/scene.hpp"
#include "flatland/event.hpp"

constexpr double dt = 1e-2;

// TODO, constexpr constructor
const mm::vec2<double> g = mm::vec2<double>{0, -9.81};

constexpr double inf_x = -1;
constexpr double sup_x = 1;
constexpr double inf_y = -1;
constexpr double sup_y = 1;

struct phys_body {

    phys_body(const mm::vec2<double>& _pos, const mm::vec2<double>& _v = {0, 0})
        : pos(_pos), v(_v) {}
    
    mm::vec2<double> pos; // position
    mm::vec2<double> v; // velocity

    void update() { // Euler-Cromer schema, it conserves total energy in time
        v += g * dt;
        pos += v * dt;
    }
};

class ball : public flat::trait::projector {
public:
    static const mm::vec2<double> init_ball_pos;

    ball( flat::scene& scene,
          mm::vec2<double> _size,
          mm::vec2<double>& _expand,
          mm::vec2<double>& _centre,
          mm::vec2<double> _pos = {0, 0})
        : body(_pos), size(_size), expand(_expand), centre(_centre),
          update_task(flat::state::get().update.delegate_task(&phys_body::update, &body)),
          sprite(scene.load_sprite("res/baseball_ball.png", this))
    {
        if (sprite == nullptr) {
            npdebug("Sprite 'ball' not found")
        }

        sprite->z = 1;
    }

    ~ball() {
        npdebug("Counts: ", sprite.use_count())
    }

    inline mm::vec2<double>& position() {
        return body.pos;
    } 

    inline mm::vec2<double>& velocity() {
        return body.v;
    } 

private:

    phys_body body;

    mm::vec2<double> size;

    // projection
    mm::vec2<double>& expand; 
    mm::vec2<double>& centre; 

    // update task
    std::shared_ptr<flat::core::task> update_task;

    // sprites
    std::shared_ptr<flat::sprite> sprite;

    virtual wsdl2::rect projection() const override {
        return wsdl2::rect{(int)((body.pos[0] + centre[0] - size[0]/2.0) / expand[0]), (int)((body.pos[1] + centre[1] - size[1]/2.0) / expand[1]), (int)(size[0] / expand[0]), (int)(-size[1] / expand[1])};
    }
};

class test {


public:

    test(const mm::vec2<double>& bounds_x, const mm::vec2<double>& bounds_y) 
    {
        flat::state& engine = flat::state::get();
        auto& scene = engine.current_scene();
        auto& win = engine.window();

        set_space(bounds_x, bounds_y);

        // determine projection
        eval_space();

        // connect events callbacks
        quit_keylist = engine.events.connect(&test::escape_quit, this);
        move_keylist = engine.events.connect(&test::keypressed, this);

        quitlist = engine.events.connect(&test::close_quit, this);

        update_task = engine.update.delegate_task(&test::update_model, this);

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
        baseball = new ball(engine.current_scene(), {0.2, 0.2}, expand, centre, ball::init_ball_pos);

        if (background == nullptr) {
            npdebug("Background is null")
        }

        // open the window
        win.open();
    }

    ~test() {
        delete baseball;
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
        auto win_size = flat::state::get().window().size();
        expand = {space_size[0]/win_size.x, -space_size[1]/win_size.y};
    }

    void escape_quit(wsdl2::event::key event) {
        if (event.type == wsdl2::event::key::action::down && 
            event.code() == SDLK_ESCAPE)
                flat::state::get().quit();
    }

    void close_quit(wsdl2::event::quit e) {
        flat::state::get().quit();
    }

    void update_model() {

        // check keys
        //key_state.update(); // not necessary
        key_state.exec_if<SDL_SCANCODE_A>([this]() {
                this->baseball->position() += mm::vec2<double>({-0.02,  0});
                });

        key_state.exec_if<SDL_SCANCODE_D>([this]() {
                this->baseball->position() += mm::vec2<double>({0.02,  0});
                });

        key_state.exec_if<SDL_SCANCODE_W>([this]() {
                this->baseball->velocity() += mm::vec2<double>({0,  0.2});
                });

        key_state.exec_if<SDL_SCANCODE_S>([this]() {
                this->baseball->velocity() += mm::vec2<double>({0,  -0.2});
                });

        // check boundaries, TODO
    }

    void keypressed(const wsdl2::event::key event) {
        if (event.type == wsdl2::event::key::action::down) {
            //npdebug("You pressed ", static_cast<char>(event.code()));
 
            if (event.code() == SDLK_ESCAPE)
                flat::state::get().quit();
        }
    }

private:

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
    ball * baseball;

    // key_state
    wsdl2::event::key::state key_state;
};

const mm::vec2<double> ball::init_ball_pos({0, sup_y * 0.8});

int main(int, char**) {

    flat::initialize();
    flat::state::create("Flatland Test");

    test game_scene({inf_x, sup_x}, {inf_y, sup_y});

    flat::run();

    return 0;
}
