#include "surface.hpp"

#include <iostream>

using namespace std;
using namespace flat;

surface::surface(const char *filename, uint32_t format, SDL_Surface *parent)
    : core::labelled(filename, true), parent(parent), hide(false)
{
    cout << "surface: loading " << filename << endl;
    sdl_surface = loadOptimizedSurface(filename, format);

    if (!sdl_surface)
    {
        cout << "Warning: could not load surface " << filename << endl;
    }

    offset = new SDL_Rect;

    offset->x = 0;
    offset->y = 0;
    offset->w = sdl_surface->w;
    offset->h = sdl_surface->h;

    viewport = new SDL_Rect;

    viewport->x = 0;
    viewport->y = 0;
    viewport->w = sdl_surface->w;
    viewport->h = sdl_surface->h;
}

surface::surface(SDL_Surface *sdl_surface, SDL_Surface *parent)
    : core::object(), parent(parent), hide(false)
{
    this->sdl_surface = new SDL_Surface(*sdl_surface);

    offset = new SDL_Rect;

    offset->x = 0;
    offset->y = 0;
    offset->w = sdl_surface->w;
    offset->h = sdl_surface->h;

    viewport = new SDL_Rect;

    viewport->x = 0;
    viewport->y = 0;
    viewport->w = sdl_surface->w;
    viewport->h = sdl_surface->h;
}

surface::surface(const surface &sprite)
    : core::object(sprite), parent(sprite.parent),
        hide(sprite.hide)
{
    offset = new SDL_Rect(*sprite.offset);

    viewport = new SDL_Rect(*sprite.viewport);

    sdl_surface = copySurface(sprite.sdl_surface);
}

surface::~surface()
{
    SDL_FreeSurface(sdl_surface);

    delete offset;
    delete viewport;
}

void surface::setOffset(int x, int y, int w, int h)
{
    offset->x = x;
    offset->y = y;

    if (w > 0)
        offset->w = w;

    if (h > 0)
        offset->h = h;
}

void surface::setViewport(int x, int y, int w, int h)
{
    viewport->x = x;
    viewport->y = y;
    viewport->w = w;
    viewport->h = h;
}

void surface::setViewport(const SDL_Rect &rect)
{
    *viewport = rect;
}


void surface::setOffset(const SDL_Rect &offset)
{
    *this->offset = offset;
}

const SDL_Rect * surface::getOffset() const
{
    return offset;
}

const SDL_Rect * surface::getViewport() const
{
    return viewport;
}

void surface::setParent(SDL_Surface *parent)
{
    this->parent = parent;
}

SDL_Surface * surface::getParent()
{
    return parent;
}

SDL_Surface * surface::getSurface()
{
    return sdl_surface;
}

void surface::setHidden(bool flag)
{
    hide = flag;
}

bool surface::isHidden() const
{
    return hide;
}

void surface::blit()
{
    if (!hide)
        SDL_BlitSurface(sdl_surface, viewport, parent, offset);
}

SDL_Surface * surface::loadOptimizedSurface(const char *filename, uint32_t format)
{
    SDL_Surface * optimized = 0;

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
