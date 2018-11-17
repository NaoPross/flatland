#ifndef __FLATSPRITER_H__
#define __FLATSPRITER_H__

#include "flatactor.h"

class FlatSprite;

class FlatSpriter : public FlatActor
{
    FlatSprite * sprite;

public:

    FlatSpriter(const char *filename, Uint32 format, FlatSpriter *parent = 0);
    FlatSpriter(SDL_Surface *surface, FlatSpriter *parent = 0);
    FlatSpriter(FlatSprite * sprite, FlatSpriter *parent = 0);

    ~FlatSpriter();

    /* Sprite modifiers */

    FlatSprite * getSprite();

    void setSprite(FlatSprite*);

    FlatActor * getParent();

    void setParent(FlatActor*);
};

#endif
