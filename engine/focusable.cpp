#include "focusable.h"
#include "flattask.h"
#include "SDL2/SDL.h"


SDL_EventCollector::SDL_EventCollector()
{
    checker = new FlatTask<SDL_EventCollector>(this, &SDL_EventCollector::collect, 0);
}

SDL_EventCollector::~SDL_EventCollector()
{
    delete checker;
}

void SDL_EventCollector::collect(void*)
{
    SDL_Event event;

    while ( SDL_PollEvent(&event) )
        stack.push_back(event);
}



SDL_EventCollector * Focusable::collector = new SDL_EventCollector();

Focusable::Focusable(bool focused) : focused(focused) 
{
    event_trigger = new FlatTask<Focusable>(this, &Focusable::serial_precall, 0); 
}

Focusable::~Focusable()
{
    delete event_trigger;
}

void Focusable::setFocused(bool flag)
{
    focused = flag;
}

bool Focusable::isFocused() const
{
    return focused;
}

void Focusable::serial_precall(void*)
{
    for (auto event : Focusable::collector->stack)
        serial_cb(event);
}

