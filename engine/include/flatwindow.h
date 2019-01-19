#ifndef __FLATWINDOW_H__
#define __FLATWINDOW_H__

#include <string>

struct window_status
{
    window_status(  unsigned char fullscreen = 0,
                    unsigned char hidden = 0,
                    unsigned char borderless = 0,
                    unsigned char resizable = 1,
                    unsigned char minimized = 0,
                    unsigned char maximized = 0,
                    unsigned char focus = 1)

        : fullscreen(fullscreen), hidden(hidden), borderless(borderless),
          resizable(resizable), minimized(minimized), maximized(maximized),
          focus(focus) {}

    unsigned char fullscreen:1;
    unsigned char hidden:1;
    unsigned char borderless:1;
    unsigned char resizable:1;
    unsigned char minimized:1;
    unsigned char maximized:1;
    unsigned char focus:1;
};

#include "flatobject.h"
#include "serial/keyfocusable.h"

class SDL_Window;
class FlatLayer;

class SDL_KeyEvent;

class FlatWindow : public FlatObject, virtual public KeyFocusable
{
    std::string title;
    window_status status;

    SDL_Rect * bounds;
    SDL_Window * sdl_window;
    SDL_Surface * screen;

    FlatLayer * main_layer;

    void key_cb(const SDL_KeyboardEvent*) override;

// TODO window calls
//protected:

    //virtual void resizeEvent();

    //virtual void quitEvent();

public:

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

    static Uint32 winstatus_to_flags(window_status);
};

#endif
