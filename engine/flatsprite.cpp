#include "flatsprite.h"

#include <iostream>

using namespace std;

FlatSprite::FlatSprite(const char *filename, Uint32 format, SDL_Surface *parent)

    : FlatObject(), parent(parent), hide(false)
{
    setID(filename);

    cout << "FlatSprite: loading " << filename << endl;
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

FlatSprite::FlatSprite(SDL_Surface *surface, SDL_Surface *parent)

    : FlatObject(), parent(parent), hide(false)
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

FlatSprite::FlatSprite(const FlatSprite &sprite)

    : FlatObject(sprite), parent(sprite.parent),
        hide(sprite.hide)
{
    offset = new SDL_Rect(*sprite.offset);

    viewport = new SDL_Rect(*sprite.viewport);

    surface = copySurface(sprite.surface);
}

FlatSprite::~FlatSprite()
{
    SDL_FreeSurface(surface);

    delete offset;
    delete viewport;
}

void FlatSprite::setOffset(int x, int y, int w, int h)
{
    offset->x = x;
    offset->y = y;

    if (w > 0)
        offset->w = w;

    if (h > 0)
        offset->h = h;
}

void FlatSprite::setViewport(int x, int y, int w, int h)
{
    viewport->x = x;
    viewport->y = y;
    viewport->w = w;
    viewport->h = h;
}

void FlatSprite::setViewport(const SDL_Rect &rect)
{
    *viewport = rect;
}


void FlatSprite::setOffset(const SDL_Rect &offset)
{
    *this->offset = offset;
}

const SDL_Rect * FlatSprite::getOffset() const
{
    return offset;
}

const SDL_Rect * FlatSprite::getViewport() const
{
    return viewport;
}

void FlatSprite::setParent(SDL_Surface *parent)
{
    this->parent = parent;
}

SDL_Surface * FlatSprite::getParent()
{
    return parent;
}

SDL_Surface * FlatSprite::getSurface()
{
    return surface;
}

void FlatSprite::setHidden(bool flag)
{
    hide = flag;
}

bool FlatSprite::isHidden() const
{
    return hide;
}

void FlatSprite::blit()
{
    if (!hide)
        SDL_BlitSurface(surface, viewport, parent, offset);
}

SDL_Surface * FlatSprite::loadOptimizedSurface(const char *filename, Uint32 format)
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

SDL_Surface * FlatSprite::copySurface(SDL_Surface *src)
{
    return SDL_ConvertSurface(src, src->format, src->flags);
}
