#include "flatland/flatland.hpp"

#include <wsdl2/event.hpp>
#include <wsdl2/wsdl2.hpp>

#include <variant>
#include <chrono>
#include <thread>


/* state singleton */
/// singleton object
static flat::state *flat_state_singleton = nullptr;

/// the event broadcast is handled by update
flat::state::state(wsdl2::renderer&) : events(update)
{
    // create an empty scene
    m_scenes.emplace();
}

flat::state& flat::state::create(wsdl2::renderer& r)
{
    static flat::state singleton = flat::state(r);

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

void flat::state::renderer(wsdl2::renderer& r)
{
    m_renderer = &r;
}

wsdl2::renderer& flat::state::renderer()
{
#ifdef DEBUG
    if (m_renderer == nullptr) {
        throw std::runtime_error("requested renderer without having it set first (nullptr)");
    }
#endif

    return *m_renderer;
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

void flat::state::pop_scene()
{
    if (m_scenes.size() > 1)
        m_scenes.pop();
}


/* free functions */

bool flat::initialize()
{
    return wsdl2::initialize();
}

void flat::quit()
{
    wsdl2::quit();
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
}
