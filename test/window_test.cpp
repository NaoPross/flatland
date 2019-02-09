#include "flatland.hpp"
#include "debug.hpp"

#include "wsdl2/event.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"
#include <SDL2/SDL.h>

#include <iostream>

using namespace flat;

int step = 0;

void gloop()
{
    if (step % 30 == 0)     
    {
        npdebug("Step: ", step)
    }

    if (step == 200)
    {
        npdebug("Step limit reached: exiting")
        quit();
    }

    if (step == 210)
    {
        npdebug("Step limit exceded: forcing to stop the loop")
        force_quit(-1);
    }

    step++;
}

void key_cb(wsdl2::event::e_key event)
{
    npdebug("Key response")

    if (event.action() == wsdl2::event::e_key::action_t::down
       && event.get() == SDLK_ESCAPE)
    {
        npdebug("ESC key pressed, exiting")
        quit();
    }
}

std::shared_ptr<core::listener<wsdl2::event::e_key>> catch_key;
std::shared_ptr<core::task> gloop_cb;

int main() {

    npdebug("Initializing window_test")
   
    // bind main loop task
    gloop_cb = main_job().delegate_task(&gloop);

    catch_key = core_channel().connect(&key_cb);

    int code = init_flatland("Window test");
    return code;
}

