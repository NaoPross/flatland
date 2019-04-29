#include "flatland.hpp"

#include "wsdl2/event.hpp"

#include <variant>
#include <chrono>
#include <thread>


bool flat::initialize() {
    return wsdl2::initialize();
}
    
void flat::quit() {
    wsdl2::quit();
}

void flat::run(flat::state& s) {
    std::chrono::time_point<std::chrono::steady_clock> start, end;

    s.running = true;

    do {
        start = std::chrono::steady_clock::now();

        { // poll events and push them into the engine's channel
            using namespace wsdl2::event;
            while (auto&& ev = poll()) {
                std::visit([&](auto&& event) {
                    s.events.emit(std::move(event));
                }, ev.value());
            }
        }

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