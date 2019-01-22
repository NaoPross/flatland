#include "flatsurface.h"

#include <iostream>

using namespace std;

FlatSurface::FlatSurface(const char *filename, uint32_t format, SDL_Surface *parent)

    : flat::core::object(), parent(parent), hide(false)
{
    setID(filename);

    cout << "FlatSurface: loading " << filename << endl;
    surface = loadOptimizedSurface(filename, format);

    if (!surface)
    {
        cout << "Warning: could not load surface " << filename << endl;
    }

    offset = new SDL_Rect;

    offset->x = 0;
    offset->y = 0;
    offset->w = surface->w;
    offset->h = surface->h;

    viewport = new SDL_Rect;

    viewport->x = 0;
    viewport->y = 0;
    viewport->w = surface->w;
    viewport->h = surface->h;
}

FlatSurface::FlatSurface(SDL_Surface *surface, SDL_Surface *parent)

    : flat::core::object(), parent(parent), hide(false)
{
    this->surface = new SDL_Surface(*surface);

    offset = new SDL_Rect;

    offset->x = 0;
    offset->y = 0;
    offset->w = surface->w;
    offset->h = surface->h;

    viewport = new SDL_Rect;

    viewport->x = 0;
    viewport->y = 0;
    viewport->w = surface->w;
    viewport->h = surface->h;
}

FlatSurface::FlatSurface(const FlatSurface &sprite)

    : flat::core::object(sprite), parent(sprite.parent),
        hide(sprite.hide)
{
    offset = new SDL_Rect(*sprite.offset);

    viewport = new SDL_Rect(*sprite.viewport);

    surface = copySurface(sprite.surface);
}

FlatSurface::~FlatSurface()
{
    SDL_FreeSurface(surface);

    delete offset;
    delete viewport;
}

void FlatSurface::setOffset(int x, int y, int w, int h)
{
    offset->x = x;
    offset->y = y;

    if (w > 0)
        offset->w = w;

    if (h > 0)
        offset->h = h;
}

void FlatSurface::setViewport(int x, int y, int w, int h)
{
    viewport->x = x;
    viewport->y = y;
    viewport->w = w;
    viewport->h = h;
}

void FlatSurface::setViewport(const SDL_Rect &rect)
{
    *viewport = rect;
}


void FlatSurface::setOffset(const SDL_Rect &offset)
{
    *this->offset = offset;
}

const SDL_Rect * FlatSurface::getOffset() const
{
    return offset;
}

const SDL_Rect * FlatSurface::getViewport() const
{
    return viewport;
}

void FlatSurface::setParent(SDL_Surface *parent)
{
    this->parent = parent;
}

SDL_Surface * FlatSurface::getParent()
{
    return parent;
}

SDL_Surface * FlatSurface::getSurface()
{
    return surface;
}

void FlatSurface::setHidden(bool flag)
{
    hide = flag;
}

bool FlatSurface::isHidden() const
{
    return hide;
}

void FlatSurface::blit()
{
    if (!hide)
        SDL_BlitSurface(surface, viewport, parent, offset);
}

SDL_Surface * FlatSurface::loadOptimizedSurface(const char *filename, uint32_t format)
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

SDL_Surface * FlatSurface::copySurface(SDL_Surface *src)
{
    return SDL_ConvertSurface(src, src->format, src->flags);
}
