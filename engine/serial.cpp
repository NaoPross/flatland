#include "serial.hpp"
#include "wsdl2/event.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

#include <variant>


void flat::serial::broadcast()
{
    using namespace wsdl2::event;

    while (auto&& ev = poll()) {
        std::visit([](auto&& event) {
            flat::core_channel().emit(std::move(event));
        }, ev.value());
    }
}

