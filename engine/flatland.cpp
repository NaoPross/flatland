#include "flatland.hpp"

#include <set>
#include <iostream>

//#include <SDL2/SDL.h>

#include <ctime>

using namespace flat;

#include "core/task.hpp"
#include "core/signal.hpp"
#include "serial.hpp"
#include "window.hpp"

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

// exit code
int exit_code = 0;

// jobs and channels

// needed to determine initialization order
class task_man
{
public:

    core::job m_job;

    core::channel core_chan;

    task_man();

private:

    /*
    * Render-base objets mapping by their integer hash
    */
    std::map<std::size_t, renderbase*> renderbases;

    // render mapping/unmapping listeners
    std::shared_ptr<core::listener<renderbase::map_pck>> render_map;
    std::shared_ptr<core::listener<renderbase::unmap_pck>> render_unmap;

    std::shared_ptr<core::task> event_cb;

    void render_map_cb(renderbase::map_pck obj)
    {
        npdebug("Constructing renderable object", obj.sender->uuid)
        renderbases.insert(std::pair(obj.sender->uuid, obj.sender));
    }

    void render_unmap_cb(renderbase::unmap_pck obj)
    {
        npdebug("Destructing renderable object ", obj.uuid)
        renderbases.erase(obj.uuid);
    }
};

task_man::task_man() : core_chan(m_job, core::priority_t::higher)
{
    // renderbase mapping
    render_map = core_chan.connect(&task_man::render_map_cb, this);
    render_unmap = core_chan.connect(&task_man::render_unmap_cb, this);

    // events calling task
    event_cb = m_job.delegate_task(&serial::broadcast, core::priority_t::max);
}

/*
 * The class is initialized in global scope
 * but the fields are initialized in the right order
 */
task_man task_manager;

/* Accessors */

core::channel& flat::core_channel()
{
    return task_manager.core_chan;
}

core::job& flat::main_job()
{
    return task_manager.m_job;
}

// force quit call type
struct force_quit_call 
{
    std::string m_reason;

    force_quit_call(const std::string& reason) : m_reason(reason) {}
};

/* Main loop */

int flat::init_flatland(const std::string& title,
                        init_predicate p, 
                        std::size_t width,
                        std::size_t height,
                        float _fps)
{
    npdebug("Flatland: Initializing flatland")

    npdebug("Flatland: Initializing SDL")

    // initialize SDL
    wsdl2::initialize();
    
    npdebug("Flatland: Initializing window")

    // init window
    
    window win(title, width, height);

    g_window = &win;
    fps = _fps;

    // call predicate
    p(win);

    npdebug("Flatland: Opening window")
    win.open();
    
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
                task_manager.m_job();

            } catch (const force_quit_call& f) {
                
                npdebug("Flatland: a force quit call was thrown")
                npdebug("Possible reason: ", f.m_reason);

                quit(exit_code);
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

    win.close();
    g_window = 0;

    npdebug("Flatland: quitting SDL")

    wsdl2::quit();

    return exit_code;
}

void flat::quit(int code)
{
    running = false;
    loop = false;
    exit_code = code;
}

void flat::force_quit(int code, const std::string& reason)
{
    exit_code = code;
    throw force_quit_call(reason);
}

