#include "flatland/flatland.hpp"

#include <wsdl2/event.hpp>
#include <wsdl2/wsdl2.hpp>

#include <variant>
#include <chrono>
#include <thread>


/* state singleton */
/// singleton object
static flat::state *flat_state_singleton = nullptr;

flat::state::~state() {
    
    wsdl2::quit();
}

/// the event broadcast is handled by update
flat::state::state(const std::string& title, 
                   std::size_t width, 
                   std::size_t height) : events(update), m_window(title, width, height) 
{
    // create an empty scene
    m_scenes.emplace();
}

flat::state& flat::state::create(const std::string& title, 
                                 std::size_t width, 
                                 std::size_t height)
{
    static flat::state singleton = flat::state(title, width, height);

#ifndef NDEBUG
    if (flat_state_singleton != nullptr) {
        throw std::logic_error(
            "flat::state::create(...) was called 2 (or more) times"
        );
    }
#endif

    flat_state_singleton = &singleton;
    return singleton;
}

flat::state& flat::state::get()
{
#ifndef NDEBUG
    if (flat_state_singleton == nullptr) {
        throw std::logic_error(
            "flat::state::create(...) was not called, "
            "singleton not initialized"
        );
    }
#endif
    return *flat_state_singleton;
}

wsdl2::window& flat::state::window()
{
    return m_window;
}

void flat::state::new_scene()
{
    pop_scene();
    m_scenes.emplace();
}

flat::scene& flat::state::current_scene()
{
    return m_scenes.top();
}

void flat::state::push_scene(flat::scene&& s)
{
    m_scenes.push(s);
}

void flat::state::push_scene(const flat::scene& s)
{
    m_scenes.push(s);
}


void flat::state::pop_scene()
{
    if (m_scenes.size() > 1)
        m_scenes.pop();
}

void flat::state::quit()
{
    state::get().running = false;
}

/* free functions */

bool flat::initialize()
{
    return wsdl2::initialize();
}

void flat::quit()
{
    flat::state::get().quit();
}

void flat::run()
{
    flat::state& s = flat::state::get();
    std::chrono::time_point<std::chrono::steady_clock> start, end;

    s.running = true;

    do {
        start = std::chrono::steady_clock::now();

        {
            // poll events and push them into the engine's channel
            using namespace wsdl2::event;
            while (auto&& ev = poll()) {
                std::visit([&](auto&& event) {
                    s.events.emit(std::move(event));
                }, ev.value());
            }
        }

        // TODO: separate update and render in different threads

        // call game update tasks
        s.update();

        // call graphics tasks
        s.render();

        end = std::chrono::steady_clock::now();

        std::this_thread::sleep_for(
            std::chrono::microseconds(1000000 / s.fps) - (end - start)
        );

    } while (s.running);

    // quit wsdl2 here
    // TODO, is it a good idea?
    wsdl2::quit();
}
