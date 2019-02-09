#ifndef __FLATLAND_H__
#define __FLATLAND_H__

#include <string>

namespace flat {

class window;

/*struct flat_status
{
    unsigned char video;
    unsigned char audio;
    unsigned char timer;
    unsigned char events;
    unsigned char joystick;
    unsigned char controller;
    unsigned char haptic;
};*/

namespace core {
    
    class job;
    class task;
    class channel;
}

class renderbase;

/*
 * Initialization
 */

struct init_predicate
{
    virtual ~init_predicate(){}

    // initialization call
    virtual void operator()(window&) {}
};

int init_flatland(const std::string& title,
                  init_predicate p = init_predicate(), 
                  std::size_t width = 800,
                  std::size_t height = 600,
                  float fps = 60);

void quit(int code = 0);
void force_quit(int code, const std::string& reason = "");

/* 
 * Engine channels 
 */

core::channel& core_channel();

/* Main job access */

core::job& main_job();

// TODO, accessors to renderbases

/* Window and status accessors */

window * current_window();

}

#endif
