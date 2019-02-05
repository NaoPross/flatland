#include "flatland.hpp"

#include <set>
#include <iostream>

//#include <SDL2/SDL.h>

#include <ctime>

using namespace flat;

#include "core/task.hpp"
#include "core/signal.hpp"
#include "window.hpp"
#include "exception.hpp"
#include "exceptions/forcequit.hpp"

#include "wsdl2/wsdl2.hpp"

/* Global fields definitions */

float flatland_dt;

window * g_window = 0;

//flat_status status;

float fps;
float fps_pause = 5;

// loopers booleans

bool running = false;
bool loop = false;

// jobs and channels

core::job mainsync_job;

core::channel * core_chan = 0;
core::channel * event_chan = 0;

std::list<std::weak_ptr<renderbase>> render_objects;

std::shared_ptr<core::listener<std::string>> cmd_listener;
std::shared_ptr<core::listener<std::shared_ptr<renderbase>, bool>> rndr_listener;

/* channels listeners callback */

void cmd_callback(std::string out)
{
    std::stringstream ss(out);
    std::string buf;
    
    while(ss >> buf) 
    {
        if (buf == "quit")
        {
            npdebug("Flatland: quit request attempted")
            flat::quit();

        } else if (buf == "echo") {
            
            ss >> buf;
            npdebug("Flatland: ", buf)
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
        render_objects.remove_if([&](const auto& v) {
            return std::owner_less<std::weak_ptr<renderbase>>()(obj, v);
        });
}


/* Functions implementation */

/*uint32_t status_to_flags(const flat_status& s)
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
}*/

/* Accessors */

core::channel * flat::core_channel()
{
    return core_chan;
}

core::channel * flat::event_channel()
{
    return event_chan;
}

core::job& flat::main_job()
{
    return mainsync_job;
}

/* Main loop */

int flat::init_flatland(window* w, float _fps)
{
    npdebug("Flatland: Initializing flatland")

    // init core channel
    
    npdebug("Flatland: Initializing core channel")
    core_chan = new core::channel(core::priority_t::max); 

    npdebug("Flatland: Initializing event channel")
    event_chan = new core::channel(core::priority_t::max);

    // bind listeners

    /*cmd_listener = core_chan->connect(cmd_callback);

    // control if print was not already connected
    if (cmd_listener == nullptr) {
        
        npdebug("Flatland: Could not connect 'cmd' listener")
        npdebug("Flatland: Do not connect to core channel another listener with filter name 'print' before flatland initialization")
        npdebug("Flatland: Aborting")
        return -1;
    }

    rndr_listener = core_chan->connect(rndrbase_callback);

    // control if print was not already connected
    if (rndr_listener == nullptr) {
        
        npdebug("Flatland: Could not connect 'render' listener")
        npdebug("Flatland: Do not connect to core channel another listener with filter name 'print' before flatland initialization")
        npdebug("Flatland: Aborting")
        return -1;
    }*/

    // init variables
    
    npdebug("Flatland: Initializing window")

    g_window = w;
    //status = s;
    fps = _fps;

    // init SDL
    
    npdebug("Flatland: Initializing SDL")
    
    /*uint32_t flags = status_to_flags(s);
    
    if ( SDL_Init(flags | SDL_INIT_NOPARACHUTE) < 0)
    {
        npdebug("Error: SDL_Init failed")
        return -1;
    }*/
    
    // initialize SDL
    wsdl2::initialize();

    // init window

    npdebug("Flatland: Opening window")
    g_window->open();
    
    /* Game loop */

    running = true;
    loop = true;

    clock_t delay = 0;

    npdebug("Flatland: Entering game-loop")

    do
    {
        do {

            flatland_dt = 1.0f / fps + delay / CLOCKS_PER_SEC;

            delay = clock();

            try {

                /* Invoke main sync job tasks */
                mainsync_job();

            } catch (const ForceQuit& f) {
                
                npdebug("Flatland: a force quit call was thrown")
                npdebug("Possible reason: ", f.m_reason);

                quit();
            }

            //SDL_Delay((uint32_t) (1000.0f / fps));
            wsdl2::delay((uint32_t) (1000.0f / fps));

            delay -= clock();

        } while (loop);
        
        //SDL_Delay((uint32_t)(1000 / fps_pause));
        wsdl2::delay((uint32_t) (1000.0f / fps_pause));
    }
    while(running);

    npdebug("Flatland: closing window")

    g_window->close();

    npdebug("Flatland: quitting SDL")

    // finalize core channel
    delete core_chan;
    delete event_chan;

    wsdl2::quit();
    //SDL_Quit();

    return 0;
}

void flat::quit()
{
    running = false;
    loop = false;
}

/*flat_status flat::flatland_status()
{
    return status;
}*/

