#ifndef __FLATWINDOW_H__
#define __FLATWINDOW_H__

#include <string>
#include "wsdl2/video.hpp"
#include "renderbase.hpp"
#include "core/collector.hpp"
#include "core/task.hpp"

namespace flat {

class window : public renderbase::collector, public wsdl2::window
{
public:

    using wsdl2::window::window;

    window( const std::string& title, 
            std::size_t width = 860, 
            std::size_t height = 640);

    // exactly the same as layer
    // but a window is not a renderbase class
    void render();
};

}
#endif
