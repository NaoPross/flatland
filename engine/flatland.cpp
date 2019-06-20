#include "flatland.hpp"

#include "wsdl2/event.hpp"

#include <variant>
#include <chrono>
#include <thread>


flat::state& flat::state::get()
{
    static state singleton;
    return singleton;
}

void flat::state::set_renderer(wsdl2::renderer& r)
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

bool flat::initialize()
{
    // instantiate singleton
    flat::state::get();

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
