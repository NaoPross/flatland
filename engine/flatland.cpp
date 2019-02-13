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

float fps;
float fps_pause = 5;

// loopers booleans

bool running = false;
bool loop_state = false;

// exit code
int exit_code = 0;

// renderer
window * main_win;

// jobs and channels

// needed to determine initialization order
class task_man
{
public:

    core::job m_job;

    core::channel core_chan;

    /*
    * Render-base objets mapping by their integer hash
    */
    std::unordered_map<std::size_t, renderbase*> renderbases;

    task_man();

private:

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

// renderer
wsdl2::renderer * renderer()
{
    return (main_win != NULL) ? &main_win->get_renderer() : NULL;
}

renderbase * find_renderable(std::size_t uuid)
{
    auto it = task_manager.renderbases.find(uuid);

    return (it == task_manager.renderbases.end()) ? NULL : (*it).second;
}

std::vector<renderbase*> find_renderable(const std::string& label)
{
    std::vector<renderbase*> out;

    for (auto& pair : task_manager.renderbases)
    {
        if (pair.second->label == label)
            out.push_back(pair.second);
    }

    return out;
}

/* Main loop */

bool flat::init()
{
    npdebug("Flatland: Initializing flatland")
    npdebug("Flatland: Initializing SDL")

    // initialize SDL
    return wsdl2::initialize();
}

int flat::loop(window& win, float _fps)
{
    fps = _fps;

    npdebug("Flatland: Opening window")
    win.open();

    main_win = &win;
    
    /* Game loop */

    running = true;
    loop_state = true;

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

        } while (loop_state);
        
        //SDL_Delay((uint32_t)(1000 / fps_pause));
        wsdl2::delay((uint32_t) (1000.0f / fps_pause));
    }
    while(running);

    npdebug("Flatland: closing window")

    win.close();

    main_win = 0;

    npdebug("Flatland: quitting SDL")

    wsdl2::quit();

    return exit_code;
}

void flat::quit(int code)
{
    running = false;
    loop_state = false;
    exit_code = code;
}

void flat::force_quit(int code, const std::string& reason)
{
    exit_code = code;
    throw force_quit_call(reason);
}

