#ifndef __FLATSERIAL_H__
#define __FLATSERIAL_H__

#include "types.hpp"
#include <vector>
#include "SDL2/SDL.h"

union SDL_Event;
class task_s;

struct SDL_EventCollector
{
    task_s * checker;
    task_s * eraser;
   
    /* Keyboard event */ 
    std::vector<SDL_Event> keyboard;

    /* Window resize or iconize event */
    std::vector<SDL_Event> window;

    /* Quit notification */
    std::vector<SDL_Event> quit;

    /* Custom events: useful for signals */
    std::vector<SDL_Event> user;

    // TODO other events

    SDL_EventCollector();
    ~SDL_EventCollector();

    void collect(void*);
    void erase(void*);

    const std::vector<SDL_Event>& getStack(uint32_t id) const;
};

namespace FlatSerial {

    extern SDL_EventCollector * collector;
}

#endif
