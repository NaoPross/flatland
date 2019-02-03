#include "flatland.hpp"

#include <set>
#include <iostream>

#include <SDL2/SDL.h>

#include <ctime>

using namespace flat;

#include "core/task.hpp"
#include "core/signal.hpp"
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
core::channel event_chan("event");

std::list<std::weak_ptr<renderbase>> render_objects;

shared_ptr<core::listener<const std::string&>> cmd_listener;
shared_ptr<core::listener<std::shared_ptr<renderbase>, bool> rndr_listener;

/* channels listeners callback */

void cmd_callback(const string& out)
{
    std::stringstream ss(out);
    std::string buf;
    
    while(ss >> buf) 
    {
        if (buf == "quit")
        {
            cout << "Flatland: quit request attempted" << endl;
            flat::quit();

        } else if (buf == "echo") {
            
            ss >> buf;
            cout << "Flatland: " << buf << endl;
        }
    }
}

void rndrbase_callback(std::shared_ptr<renderbase> obj, bool insert)
{
    if (insert)
    {
        // avoid doubling objects
        for (auto w : render_objects)
        {
            if (w.lock().get() == obj.get())
                return;
        }

        render_objects.push_back(obj);

    } else
        render_objects.remove(obj);
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

core::channel& flat::event_channel()
{
    return event_chan;
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

        cerr << "Flatland: Could not map 'core' channel" << endl;
        cerr << "Flatland: Do not call another channel 'core' before flatland initialization" << endl;
        cerr << "Flatland: Aborting" << endl;
        return -1;
    }

    if (!event_chan.start(core::priority_t::max, &mainsync_job)) {

        cerr << "Flatland: Could not map 'event' channel" << endl;
        cerr << "Flatland: Do not call another channel 'event' before flatland initialization" << endl;
        cerr << "Flatland: Aborting" << endl;
        return -1;
    }

    // bind listeners

    cmd_listener = core_chan.connect(cmd_callback);

    // control if print was not already connected
    if (cmd_listener == nullptr) {
        
        cout << "Flatland: Could not connect 'cmd' listener" << endl;
        cout << "Flatland: Do not connect to core channel another listener with filter name 'print' before flatland initialization" << endl;
        cout << "Flatland: Aborting" << endl;
        return -1;
    }

    rndr_listener = core_chan.connect(rndr_callback);

    // control if print was not already connected
    if (!rndr_listener) {
        
        cout << "Flatland: Could not connect 'render' listener" << endl;
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

    // finalize core channel
    core_chan.finalize();
    event_chan.finalize();

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

