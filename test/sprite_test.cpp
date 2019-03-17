#include "flatland.hpp"
#include "debug.hpp"

#include "wsdl2/event.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"

#include "window.hpp"
#include "sprite.hpp"

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

    npdebug("Initializing sprite_test")
   
    // bind main loop task
    main_job().add_task(&gloop);

    auto catch_key = core_channel().connect(&key_cb);

    window * win = 0;

    if (!init([&](){ 
        win = new window("Window test", 800, 600); 
        win->open();
        return win;
    }))
    {
        npdebug("Cannot initialize flatland")
        return -1;
    }

    // streaming texture initialization
    //
    /*npdebug("Creating the texture")

    auto tex = texloader::create("soos", 200, 200, wsdl2::pixelformat::format::unknown, wsdl2::texture::access::streaming);

    if (tex == nullptr)
        return -1;

    npdebug("Attach a new sprite to the window, step 1")

    win->attach<sprite>(tex, wsdl2::rect{200, 300, 100, 100});*/

    // sprite initialization
    //
    
    npdebug("Load the image")

    sprite * try_sprite = sprite::from_file("test/res/chiara.bmp", wsdl2::rect{100, 100, 300, 300});

    if (try_sprite == NULL)
        return -1;

    npdebug("Attach a new sprite to the window, step 2")

    win->attach(try_sprite);

    npdebug("Attach a new sprite to the window, step 2 succeded")

    return loop();
}
