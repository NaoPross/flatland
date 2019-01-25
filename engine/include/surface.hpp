#ifndef __FLATSURFACE_H__
#define __FLATSURFACE_H__

#include "core/object.hpp"
#include "core/labelled.hpp"
#include <SDL2/SDL.h>

namespace flat {

class surface : virtual public core::object, virtual public core::labelled
{
    SDL_Surface * sdl_surface;
    SDL_Surface * parent;

    SDL_Rect * offset;
    SDL_Rect * viewport;

    bool hide;

public:

    surface(const char *filename, uint32_t format = SDL_PIXELFORMAT_RGBA32, SDL_Surface *parent = 0);
    surface(SDL_Surface *surf, SDL_Surface *parent = 0);

    surface(const surface&);

    ~surface();

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

    static SDL_Surface * loadOptimizedSurface(const char* filename, uint32_t format);

    static SDL_Surface * copySurface(SDL_Surface*);
};

}

#endif
