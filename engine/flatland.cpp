#include "flatland.h"

#include <set>
#include <iostream>

#include <SDL2/SDL.h>

#include <ctime>

using namespace std;

#include "flattask.h"
#include "flatsignal.h"
#include "flatwindow.h"

float flatland_dt;

set<FlatObject*> objects;

FlatWindow * window = 0;
SignalChannel * core = 0;

gameloop loop_function;

flat_status status;

float fps;
float fps_pause = 5;

Uint32 status_to_flags(const flat_status& s)
{
    Uint32 flags = 0;

    if (s.audio)
        flags |= SDL_INIT_AUDIO;
    if (s.timer)
        flags |= SDL_INIT_TIMER;
    if (s.video)
        flags |= SDL_INIT_VIDEO;
    if (s.joystick)
        flags |= SDL_INIT_JOYSTICK;
    if (s.haptic)
        flags |= SDL_INIT_HAPTIC;
    if (s.controller)
        flags |= SDL_INIT_GAMECONTROLLER;
    if (s.events)
        flags |= SDL_INIT_EVENTS;
    if (s.haptic)
        flags |= SDL_INIT_HAPTIC;
    if (s.controller)
        flags |= SDL_INIT_GAMECONTROLLER;
    if (s.events)
        flags |= SDL_INIT_EVENTS;
    
    return flags; 
}

/* Listen to simple quit calls */
class QuitListener : public FlatListener
{
    virtual void callback(FlatObject*, void*) override
    {
        /* Order to quit */
        quit_flatland();        
    }

public:

    QuitListener()
    {
        addFilter("quit");
        core->connect(this);
    }
};

int init_flatland(FlatWindow* w, gameloop loop, const flat_status& s, float _fps)
{
    cout << "Flatland: Initializing flatland" << endl;

    // Init core channel 

    core = new SignalChannel("core");
    QuitListener quitter;

    // init variables
    
    cout << "Flatland: Initializing window" << endl;

    window = w;
    loop_function = loop;
    status = s;
    fps = _fps;

    // init SDL
    
    cout << "Flatland: Initializing SDL" << endl;
    
    Uint32 flags = status_to_flags(s);
    
    if ( SDL_Init(flags | SDL_INIT_NOPARACHUTE) < 0)
    {
        cout << "Error: SDL_Init failed" << endl;
        return -1;
    }

    // init window

    cout << "Flatland: Opening window" << endl;
    window->open();
    
    /* Game loop */

    status.running = 1;
    status.loop = 1;

    clock_t delay = 0;

    cout << "Flatland: Entering game-loop" << endl;

    do
    {
        do {

            flatland_dt = 1.0f / fps + delay / CLOCKS_PER_SEC;

            delay = clock();

            /* Execute loop function */
            loop_function(flatland_dt);

            /* Execute tasks */
            task_s::executeAll();

            SDL_Delay((Uint32) (1000.0f / fps));

            delay -= clock();

        } while (status.loop);
        
        SDL_Delay((Uint32)(1000 / fps_pause));
    }
    while(status.running);

    cout << "Flatland: destroying core channel" << endl;

    delete core;
    core = 0;

    cout << "Flatland: quitting SDL" << endl;

    SDL_Quit();

    return status.error;
}

void quit_flatland()
{
    status.running = 0;
    status.loop = 0;
}

flat_status flatland_status()
{
    return status;
}

SignalChannel * getCoreChannel()
{
    return core;
}

