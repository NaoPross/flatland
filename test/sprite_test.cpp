#include "flatland.hpp"
#include "debug.hpp"

#include "wsdl2/event.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"

#include "window.hpp"

#include <iostream>

using namespace flat;

int step = 0;

void gloop()
{
    if (step % 500 == 0)     
    {
        npdebug("Step: ", step)
    }

    if (step == 10000)
    {
        npdebug("Step limit reached: exiting")
        quit();
    }

    if (step == 10010)
    {
        npdebug("Step limit exceded: forcing to stop the loop")
        force_quit(-1);
    }

    step++;
}

void key_cb(const wsdl2::event::key event)
{
    npdebug("Key response")

    if (event.type == wsdl2::event::key::action::down
       && event.keysym.sym == SDLK_ESCAPE)
    {
        npdebug("ESC key pressed, exiting")
        quit();
    }
}

int main() {

    npdebug("Initializing window_test")
   
    // bind main loop task
    main_job().add_task(&gloop);

    auto catch_key = core_channel().connect(&key_cb);

    if (!init())
    {
        npdebug("Cannot initialize flatland")
        return -1;
    }

    window win("Window test", 800, 600);

    // sprite section

    return loop(win);
}
