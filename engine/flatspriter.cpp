#include "flatspriter.h"

#include "flatsprite.h"
#include "flatwindow.h"
#include "flatland.h"

FlatSpriter::FlatSpriter(const char *filename, Uint32 format, FlatSpriter *parent)

    : FlatActor(parent)
{
    SDL_Surface * base;

    if (parent == 0)
    {   
        // TODO, default parent
    } else
        base = parent->getSprite()->getSurface();

    sprite = new FlatSprite(filename, format, base);

    if (sprite == 0)
    {
        // TODO, error: failed to load
    }
}

FlatSpriter::FlatSpriter(SDL_Surface *surface, FlatSpriter *parent)

    : FlatActor(parent)
{
    SDL_Surface * base;

    if (parent == 0)
    {   
        // TODO, default parent
    } else
        base = parent->getSprite()->getSurface();

    if (surface == 0)
    {
        // TODO, default surface
    }

    sprite = new FlatSprite(surface, base);
}

FlatSpriter::FlatSpriter(FlatSprite * sprite, FlatSpriter *parent)

    : FlatActor(parent)
{
    if (parent == 0)
    {
        // TODO, screen surface
    }

    if (sprite == 0)
    {
        // TODO, default sprite
    }

    this->sprite = new FlatSprite(*sprite);
}

FlatSpriter::~FlatSpriter()
{
    delete sprite;
}

FlatSprite * FlatSpriter::getSprite()
{
    return sprite;
}

void FlatSpriter::setSprite(FlatSprite *sprite)
{
    
}

FlatActor * FlatSpriter::getParent()
{

}

void FlatSpriter::setParent(FlatActor*)
{

}

