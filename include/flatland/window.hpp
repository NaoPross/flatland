#pragma once

#include "trait/renderable.hpp"

#include <wsdl2/video.hpp>

namespace flat
{
    class window : public rendergroup, public wsdl2::window
    {
    public:
        using wsdl2::window::window;

        window(const std::string& title,
               std::size_t width = 860,
               std::size_t height = 640);

        void render() const override;
    };

}
