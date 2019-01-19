#include "flatserial.h"
#include "flattask.h"

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
    {
        switch(event.type)
        {

        /* Keyboard */
        case SDL_KEYDOWN:
            keyboard.push_back(event);
            break;

        case SDL_KEYUP:
            keyboard.push_back(event);
            break;

        /* Window */
        case SDL_WINDOWEVENT:
            window.push_back(event);
            break;

        /* Quit */
        case SDL_QUIT:
            quit.push_back(event);
            break;

        /* User */
        case SDL_USEREVENT:
            user.push_back(event);
            break;
        }

        // TODO other events
    }
}

const std::vector<SDL_Event>& SDL_EventCollector::getStack(Uint32 id) const
{
    switch(id)
    {
        {

        /* Keyboard */
        case SDL_KEYDOWN:
            return keyboard;

        case SDL_KEYUP:
            return keyboard;

        /* Window */
        case SDL_WINDOWEVENT:
            return window;

        /* Quit */
        case SDL_QUIT:
            return quit;

        /* User */
        case SDL_USEREVENT:
            return user;

        }

        // TODO other events
    }
}

SDL_EventCollector * FlatSerial::collector = new SDL_EventCollector();

