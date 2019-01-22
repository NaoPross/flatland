#include "serial/keyfocusable.h"
#include "SDL2/SDL.h"

void KeyFocusable::serial_cb(const SDL_Event *event)
{
    key_cb(&event->key);
}

uint32_t KeyFocusable::stackID() const
{
    // Set SDL_KEYDOWN by default
    return SDL_KEYDOWN;
}

