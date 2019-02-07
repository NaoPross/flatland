#ifndef __FLATWINDOW_H__
#define __FLATWINDOW_H__

#include <string>
#include "wsdl2/video.hpp"
#include "renderbase.hpp"
#include "core/collector.hpp"

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
    /*std::string title;
    window_status status;
private:
    std::string m_title;
    window_status m_status;

    SDL_Rect * m_bounds;
    SDL_Window * m_sdl_window;
    SDL_Surface * m_screen;

    FlatLayer * m_main_layer;

    void key_cb(const SDL_KeyboardEvent*) override;*/

// TODO window calls
//protected:

    //virtual void resizeEvent();

    //virtual void quitEvent();

/*public:

    FlatWindow(int x, int y,
               int width, int height, 
               const std::string& title, 
               window_status status = window_status());

    FlatWindow( SDL_Rect *bounds, const std::string& title,
                window_status status = window_status());

    FlatWindow( int width, int height,
                const std::string &title,
                window_status status = window_status());

    FlatWindow(const FlatWindow&);

    ~FlatWindow();

    int open();
    void close();

    int getWidth() const;
    int getHeight() const;

    const SDL_Rect * getBounds() const;

    SDL_Window * getSDLWindow();
    SDL_Surface * getScreenSurface();

    const std::string& getTitle() const;

    void setTitle(const std::string&);

    window_status getWindowStatus() const;

    void setWindowStatus(window_status);

    static uint32_t winstatus_to_flags(window_status);*/

public:

    using wsdl2::window::window;

    // exactly the same as layer
    // but a window is not a renderbase class
    void render();
};

}
#endif
