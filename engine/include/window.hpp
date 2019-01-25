#pragma once

#include "wsdl2/video.hpp"

#include "core/object.hpp"
#include "serial/keyfocusable.hpp"

#include <string>

namespace flat {
    class window : virtual public core::object, public wsdl2::window {
    public:
        window() = delete;
        window(const window& other) = delete;
        window(const std::string& title, std::size_t width, std::size_t height);
        ~window();

    private:
        // std::multiset<layer> m_layers;
    };

}
