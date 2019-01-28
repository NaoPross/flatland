#include "flatland.hpp"

#include <set>
#include <iostream>

#include <SDL2/SDL.h>

#include <ctime>

using namespace std;
using namespace flat;

#include "core/task.hpp"
#include "core/signal.hpp"
#include "object.hpp"
#include "window.hpp"
#include "exception.hpp"
#include "exceptions/forcequit.hpp"

/* Global fields definitions */

float flatland_dt;

FlatWindow * window = 0;

flat_status status;

float fps;
float fps_pause = 5;

core::job mainsync_job;

core::channel core_chan("core");

core::listener::ptr quit_listener;
core::listener::ptr print_listener;

/* channels listeners callback */

void quit_callback(const object *sender, core::signal::package)
{
    cout << "Flatland: quit request attempted" << endl;
    flat::quit();
}

void print_callback(const object *sender, core::signal::package p)
{
    const char * out = p.get<const char>();
    string * sout = p.get<string>();

    if (!out)
        cout << "Flatland: " << out << endl;
    else if (!sout)
        cout << "Flatland: " << sout << endl;

    // else fail silently
}

/* Functions implementation */

uint32_t status_to_flags(const flat_status& s)
{
    uint32_t flags = 0;

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

/* Accessors */

core::channel& flat::core_channel()
{
    return core_chan;
}

core::job& flat::main_job()
{
    return mainsync_job;
}

/* Main loop */

int flat::init_flatland(FlatWindow* w, const flat_status& s, float _fps)
{
    cout << "Flatland: Initializing flatland" << endl;

    // init core channel
    
    cout << "Flatland: Initializing core channel" << endl;
   
    // start core channel
    // assure that NO OTHER core channel was present before initialization
    if (!core_chan.start(core::priority_t::max, &mainsync_job)) {

        cout << "Flatland: Could not map 'core' channel" << endl;
        cout << "Flatland: Do not call another channel 'core' before flatland initialization" << endl;
        cout << "Flatland: Aborting" << endl;
        return -1;
    }

    // bind listeners
    
    quit_listener = core_chan.connect(quit_callback, initializer_list<string>({string("quit")}));

    // control if quit was not already connected
    if (!quit_listener) {
        
        cout << "Flatland: Could not connect 'quit' listener" << endl;
        cout << "Flatland: Do not connect to core channel another listener with filter name 'quit' before flatland initialization" << endl;
        cout << "Flatland: Aborting" << endl;
        return -1;
    }

    print_listener = core_chan.connect(print_callback, initializer_list<string>({string("print")}));

    // control if print was not already connected
    if (!print_listener) {
        
        cout << "Flatland: Could not connect 'print' listener" << endl;
        cout << "Flatland: Do not connect to core channel another listener with filter name 'print' before flatland initialization" << endl;
        cout << "Flatland: Aborting" << endl;
        return -1;
    }

    // init variables
    
    cout << "Flatland: Initializing window" << endl;

    window = w;
    status = s;
    fps = _fps;

    // init SDL
    
    cout << "Flatland: Initializing SDL" << endl;
    
    uint32_t flags = status_to_flags(s);
    
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

            try {

                /* Invoke main sync job tasks */
                mainsync_job();

            } catch (const ForceQuit& f) {
                
                cerr << "Flatland: a force quit call was thrown" << endl;
                cerr << "Possible reason: " << f.m_reason << endl;

                quit();
            }

            SDL_Delay((uint32_t) (1000.0f / fps));

            delay -= clock();

        } while (status.loop);
        
        SDL_Delay((uint32_t)(1000 / fps_pause));
    }
    while(status.running);

    cout << "Flatland: closing window" << endl;

    window->close();

    cout << "Flatland: quitting SDL" << endl;

    SDL_Quit();

    return status.error;
}

void flat::quit()
{
    status.running = 0;
    status.loop = 0;
}

flat_status flat::flatland_status()
{
    return status;
}

