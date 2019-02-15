#ifndef __FLATWINDOW_H__
#define __FLATWINDOW_H__

#include <string>
#include "wsdl2/video.hpp"
#include "renderbase.hpp"
#include "core/collector.hpp"
#include "core/task.hpp"

//class SDL_Window;
//class SDL_KeyEvent;

namespace flat {

/*

struct window_status
{
    window_status(  unsigned char _fullscreen = 0,
                    unsigned char _hidden = 0,
                    unsigned char _borderless = 0,
                    unsigned char _resizable = 1,
                    unsigned char _minimized = 0,
                    unsigned char _maximized = 0,
                    unsigned char _focus = 1)

        : fullscreen(_fullscreen), hidden(_hidden), borderless(_borderless),
          resizable(_resizable), minimized(_minimized), maximized(_maximized),
          focus(_focus) {}

    unsigned char fullscreen;
    unsigned char hidden;
    unsigned char borderless;
    unsigned char resizable;
    unsigned char minimized;
    unsigned char maximized;
    unsigned char focus;
};*/

class window : public renderbase::collector, public wsdl2::window
{
    std::shared_ptr<core::task> rendering_task;

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
