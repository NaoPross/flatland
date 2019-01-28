#include "window.hpp"

#include <SDL2/SDL.h>
#include <iostream>
#include "layer.hpp"
#include "core/signal.hpp"

using namespace flat;

FlatWindow::FlatWindow( int x, int y,
                        int width, int height, 
                        const std::string& title, 
                        window_status status)

    :   m_title(title), m_status(status),
        m_sdl_window(nullptr), m_screen(nullptr)
{
    m_bounds = new SDL_Rect;

    m_bounds->x = x;
    m_bounds->y = y;
    m_bounds->w = width; 
    m_bounds->h = height;

    m_main_layer = new FlatLayer(nullptr);
}

FlatWindow::FlatWindow( SDL_Rect *bounds, 
                        const std::string& title,
                        window_status status)

    : FlatWindow(bounds->x, bounds->y, bounds->w, bounds->h, title, status)
{}

FlatWindow::FlatWindow( int width, int height,
                        const std::string &title,
                        window_status status)

    :   FlatWindow(SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                    width, height,
                    title, status)
{
}

FlatWindow::FlatWindow(const FlatWindow& win)
    : flat::object(win), Focusable(false),
      m_title(win.m_title), m_status(win.m_status),
      m_sdl_window(nullptr), m_screen(nullptr)
{
    m_bounds = new SDL_Rect;

    *m_bounds = *win.m_bounds;

    m_main_layer = new FlatLayer(nullptr);
}

FlatWindow::~FlatWindow()
{
    close();

    delete m_bounds;
    delete m_main_layer;
}

int FlatWindow::open()
{
    uint32_t win_flags = winstatus_to_flags(m_status);
    
    m_sdl_window = SDL_CreateWindow(m_title.c_str(), 
                                  m_bounds->x,
                                  m_bounds->y,
                                  m_bounds->w,
                                  m_bounds->h,
                                  win_flags);

    if (m_sdl_window == nullptr)
    {
        std::cout << "Error: failed to initialize window" << std::endl;
        // throw exception
        return -1;
    }

    m_screen = SDL_GetWindowSurface(m_sdl_window);

    if (m_screen == nullptr) 
    {
		std::cout << "Error: SDL_SetVideoMode failed" << std::endl;
        // throw exception
        return -1;
	}

    return 0;
}

void FlatWindow::close()
{
    if (m_screen != nullptr) {
        SDL_FreeSurface(m_screen);
        m_screen = nullptr;
    }

    if (m_sdl_window != nullptr) {
        SDL_DestroyWindow(m_sdl_window);
        m_sdl_window = nullptr;
    }
}

int FlatWindow::getWidth() const
{
    return m_bounds->w;
}

int FlatWindow::getHeight() const
{
    return m_bounds->h;
}

const SDL_Rect * FlatWindow::getBounds() const
{
    return m_bounds;
}

SDL_Window * FlatWindow::getSDLWindow()
{
    return m_sdl_window;
}

SDL_Surface * FlatWindow::getScreenSurface()
{
    return m_screen;
}

const std::string& FlatWindow::getTitle() const
{
    return m_title;
}

void FlatWindow::setTitle(const std::string& title)
{
   m_title = title; 
}

window_status FlatWindow::getWindowStatus() const
{
    return m_status;
}

void FlatWindow::setWindowStatus(window_status status)
{
    m_status = status;
}

void FlatWindow::key_cb(const SDL_KeyboardEvent *event)
{
    // TODO Default escape exits

    if (event->type == SDL_KEYDOWN && event->keysym.sym == SDLK_ESCAPE) 
    {
        /* Close window */
        close();

        /* Say flatland to quit */
        // flat::core::signal quit(this, "quit", 0, 0xff);
        // quit.emit("core");
    }
}

uint32_t FlatWindow::winstatus_to_flags(window_status s)
{
    uint32_t flags = 0;

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

