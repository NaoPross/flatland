#ifndef __TYPES_H__
#define __TYPES_H__

/* Flatland types */

namespace flat
{
    class object;

    class FlatActor;
    class FlatSprite;

    class Focusable;

    class KeyFocusable;
    class MouseFocusable;

    class FlatEvent;
    class EventListener;
    class Eventrigger;

    class FlatWindow;

    class FlatSpriter;
    class FlatMultiSpriter;
}


/* SDL types */

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

struct SDL_Surface;
struct SDL_Rect;

union SDL_Event;

#endif
