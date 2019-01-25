#include "flatserial.h"
#include "core/task.hpp"

SDL_EventCollector::SDL_EventCollector()
{
    /* Checker task, pre-process, maximum priority */
    checker = new flat::core::task<SDL_EventCollector>(this, &SDL_EventCollector::collect, 0, true, 0);

    /* Eraser task, post-process, minimum priority */
    eraser = new flat::core::task<SDL_EventCollector>(this, &SDL_EventCollector::erase, 0, false, 0xff);
}

SDL_EventCollector::~SDL_EventCollector()
{
    delete checker;
    delete eraser;
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

void SDL_EventCollector::erase(void*)
{
    keyboard.clear();
    window.clear();
    quit.clear();
    user.clear();
}

const std::vector<SDL_Event>& SDL_EventCollector::getStack(uint32_t id) const
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

