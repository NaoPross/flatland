#ifndef __FLATSPRITE_H__
#define __FLATSPRITE_H__

#include "flatobject.h"
#include <SDL2/SDL.h>

class FlatSprite : public FlatObject
{
    SDL_Surface * surface;
    SDL_Surface * parent;

    SDL_Rect * offset;
    SDL_Rect * viewport;

    bool hide;

public:

    FlatSprite(const char *filename, Uint32 format = SDL_PIXELFORMAT_RGBA32, SDL_Surface *parent = 0);
    FlatSprite(SDL_Surface *surface, SDL_Surface *parent = 0);

    FlatSprite(const FlatSprite&);

    ~FlatSprite();

    void setOffset(int, int, int w = -1, int h = -1);

    void setOffset(const SDL_Rect&);

    void setViewport(int x = 0, int y = 0, int w = -1, int h = -1);

    void setViewport(const SDL_Rect&);

    const SDL_Rect * getOffset() const;

    const SDL_Rect * getViewport() const;

    void setParent(SDL_Surface *parent);

    SDL_Surface * getParent();

    SDL_Surface * getSurface();

    void setHidden(bool);

    bool isHidden() const;

    void blit();

    static SDL_Surface * loadOptimizedSurface(const char* filename, Uint32 format);

    static SDL_Surface * copySurface(SDL_Surface*);
};

#endif
