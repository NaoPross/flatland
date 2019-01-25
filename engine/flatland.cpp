#include "flatland.hpp"
#include "core/task.hpp"
#include "core/signal.hpp"
#include "debug.hpp"

#include "window.hpp"
#include "exception.hpp"
#include "exceptions/forcequit.hpp"

#include "wsdl2/wsdl2.hpp"
#include "wsdl2/video.hpp"

#include <set>
#include <iostream>
#include <ctime>

extern "C" {
#include <SDL2/SDL.h>
}

using namespace flat;

static std::set<flat::core::object*> objects;

static bool running;
static core::job game;


void initialize(std::function<void()> loop) {
    wsdl2::initialize();
    game.add_task(static_cast<core::task::callback>(loop));

    npdebug("initialized flatland");
}

void run(unsigned framerate) {
    running = true;

    do {
        game();
        wsdl2::delay(1.0 / static_cast<double>(framerate));
    } while(running);
}

void quit()
{
    wsdl2::quit();
    npdebug("deinitialized (quit) flatland");
}
