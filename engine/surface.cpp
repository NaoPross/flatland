#include "surface.hpp"

#include <iostream>

using namespace wsdl2;

surface::surface(const char *filename, uint32_t format, SDL_Surface *parent)
    : m_parent(parent), m_hide(false)
{
    std::cout << "surface: loading " << filename << std::endl;
    m_sdl_surface = loadOptimizedSurface(filename, format);

    if (!m_sdl_surface)
    {
        std::cout << "Warning: could not load surface " << filename << std::endl;
    }

    m_offset = new SDL_Rect;

    m_offset->x = 0;
    m_offset->y = 0;
    m_offset->w = m_sdl_surface->w;
    m_offset->h = m_sdl_surface->h;

    m_viewport = new SDL_Rect;

    m_viewport->x = 0;
    m_viewport->y = 0;
    m_viewport->w = m_sdl_surface->w;
    m_viewport->h = m_sdl_surface->h;
}

surface::surface(SDL_Surface *sdl_surface, SDL_Surface *parent)
    : m_parent(parent), m_hide(false)
{
    m_sdl_surface = new SDL_Surface(*sdl_surface);

    m_offset = new SDL_Rect;

    m_offset->x = 0;
    m_offset->y = 0;
    m_offset->w = sdl_surface->w;
    m_offset->h = sdl_surface->h;

    m_viewport = new SDL_Rect;

    m_viewport->x = 0;
    m_viewport->y = 0;
    m_viewport->w = sdl_surface->w;
    m_viewport->h = sdl_surface->h;
}

surface::surface(const surface& sprite)
    : m_parent(sprite.m_parent), m_hide(sprite.m_hide)
{
    m_offset = new SDL_Rect(*sprite.m_offset);
    m_viewport = new SDL_Rect(*sprite.m_viewport);
    m_sdl_surface = copySurface(sprite.m_sdl_surface);
}

surface::~surface()
{
    SDL_FreeSurface(m_sdl_surface);

    delete m_offset;
    delete m_viewport;
}

void surface::setOffset(int x, int y, int w, int h)
{
    m_offset->x = x;
    m_offset->y = y;

    if (w > 0)
        m_offset->w = w;

    if (h > 0)
        m_offset->h = h;
}

void surface::setViewport(int x, int y, int w, int h)
{
    m_viewport->x = x;
    m_viewport->y = y;
    m_viewport->w = w;
    m_viewport->h = h;
}

void surface::setViewport(const SDL_Rect &rect)
{
    *m_viewport = rect;
}


void surface::setOffset(const SDL_Rect &offset)
{
    *m_offset = offset;
}

const SDL_Rect * surface::getOffset() const
{
    return m_offset;
}

const SDL_Rect * surface::getViewport() const
{
    return m_viewport;
}

void surface::setParent(SDL_Surface *parent)
{
    m_parent = parent;
}

SDL_Surface * surface::getParent()
{
    return m_parent;
}

SDL_Surface * surface::getSurface()
{
    return m_sdl_surface;
}

void surface::setHidden(bool flag)
{
    m_hide = flag;
}

bool surface::isHidden() const
{
    return m_hide;
}

void surface::blit()
{
    if (!m_hide)
        SDL_BlitSurface(m_sdl_surface, m_viewport, m_parent, m_offset);
}

SDL_Surface * surface::loadOptimizedSurface(const char *filename, uint32_t format)
{
    SDL_Surface * optimized = nullptr;
    SDL_Surface * loaded = SDL_LoadBMP(filename);

    if (loaded)
    {
        optimized = SDL_ConvertSurfaceFormat(loaded, format, 0);
        SDL_FreeSurface(loaded);
    }

    return optimized;
}

SDL_Surface * surface::copySurface(SDL_Surface *src)
{
    return SDL_ConvertSurface(src, src->format, src->flags);
}
