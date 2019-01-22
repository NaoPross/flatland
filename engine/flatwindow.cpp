#include "flatwindow.h"

#include <SDL2/SDL.h>
#include <iostream>
#include "flatlayer.h"
#include "signal.h"

using namespace std;

FlatWindow::FlatWindow( int x, int y,
                        int width, int height, 
                        const string& title, 
                        window_status status)

    :   title(title), status(status),
        sdl_window(0), screen(0)
{
    setID(title.c_str());

    bounds = new SDL_Rect;
    
    bounds->x = x;
    bounds->y = y;
    bounds->w = width; 
    bounds->h = height;

    main_layer = new FlatLayer(0);
}

FlatWindow::FlatWindow( SDL_Rect *bounds, 
                        const string& title,
                        window_status status)

    : FlatWindow(bounds->x, bounds->y, bounds->w, bounds->h, title, status)
{
}

FlatWindow::FlatWindow( int width, int height,
                        const string &title,
                        window_status status)

    :   FlatWindow( SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                    width, height,
                    title, status)
{
}

FlatWindow::FlatWindow(const FlatWindow& win)

    :   flat::core::object(win),
        title(win.title), status(win.status),
        sdl_window(0), screen(0)
{
    bounds = new SDL_Rect;

    *bounds = *win.bounds;

    main_layer = new FlatLayer(0);
}

FlatWindow::~FlatWindow()
{
    close();

    delete bounds;
    delete main_layer;
}

int FlatWindow::open()
{
    Uint32 win_flags = winstatus_to_flags(status);
    
    sdl_window = SDL_CreateWindow(  title.c_str(), 
                                    bounds->x,
                                    bounds->y,
                                    bounds->w,
                                    bounds->h,
                                    win_flags);

    if (sdl_window == 0)
    {
        cout << "Error: failed to initialize window" << endl;
        // throw exception
        return -1;
    }

    screen = SDL_GetWindowSurface(sdl_window);

    if (screen == 0) 
    {
		cout << "Error: SDL_SetVideoMode failed" << endl;
        // throw exception
        return -1;
	}

    return 0;
}

void FlatWindow::close()
{
    if (screen != 0)
    {
        SDL_FreeSurface(screen);
        screen = 0;
    }

    if (sdl_window != 0)
    {
        SDL_DestroyWindow(sdl_window);
        sdl_window = 0;
    }
}

int FlatWindow::getWidth() const
{
    return bounds->w;
}

int FlatWindow::getHeight() const
{
    return bounds->h;
}

const SDL_Rect * FlatWindow::getBounds() const
{
    return bounds;
}

SDL_Window * FlatWindow::getSDLWindow()
{
    return sdl_window;
}

SDL_Surface * FlatWindow::getScreenSurface()
{
    return screen;
}

const std::string& FlatWindow::getTitle() const
{
    return title;
}

void FlatWindow::setTitle(const std::string& title)
{
   this->title = title; 
}

window_status FlatWindow::getWindowStatus() const
{
    return status;
}

void FlatWindow::setWindowStatus(window_status status)
{
    this->status = status;
}

void FlatWindow::key_cb(const SDL_KeyboardEvent *event)
{
    // TODO Default escape exits

    if (event->type == SDL_KEYDOWN && event->keysym.sym == SDLK_ESCAPE) 
    {
        /* Close window */
        close();

        /* Say flatland to quit */
        FlatSignal quit(this, "quit", 0, 0xff);
        quit.emit("core");
    }
}

Uint32 FlatWindow::winstatus_to_flags(window_status s)
{
    Uint32 flags = 0;

    if (s.fullscreen)
        flags |= SDL_WINDOW_FULLSCREEN;
    if (s.hidden)
        flags |= SDL_WINDOW_HIDDEN;
    if (s.borderless)
        flags |= SDL_WINDOW_BORDERLESS;
    if (s.resizable)
        flags |= SDL_WINDOW_RESIZABLE;
    if (s.minimized)
        flags |= SDL_WINDOW_MINIMIZED;
    if (s.maximized)
        flags |= SDL_WINDOW_MAXIMIZED;
    if (s.focus)
        flags |= SDL_WINDOW_INPUT_GRABBED;

    return flags;
}

