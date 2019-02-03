#ifndef __FLATLAND_H__
#define __FLATLAND_H__

namespace flat {

class FlatWindow;

struct flat_status
{

    flat_status(unsigned char _video = 1,
                unsigned char _audio = 1,
                unsigned char _timer = 1,
                unsigned char _events = 1,
                unsigned char _joystick = 0,
                unsigned char _controller = 0,
                unsigned char _haptic = 0,
                unsigned char _error = 0,
                unsigned char _running = 0,
                unsigned char _loop = 0)

        : video(_video), audio(_audio), timer(_timer), events(_events),
          joystick(_joystick), controller(_controller), haptic(_haptic),
          error(_error), running(_running), loop(_loop) {}

    unsigned char video;
    unsigned char audio;
    unsigned char timer;
    unsigned char events;
    unsigned char joystick;
    unsigned char controller;
    unsigned char haptic;
    unsigned char error;
    unsigned char running;
    unsigned char loop;
};

int init_flatland(FlatWindow*, const flat_status&, float fps = 60);
void quit();

namespace core {
    
    class job;
    class task;
    class channel;
}

class renderbase;

/* Engine channels */

core::channel& core_channel();
core::channel& event_channel();

/* Main job access */

core::job& main_job();

// TODO, accessors to renderbases

/* Window and status accessors */

FlatWindow * getFlatWindow();

flat_status flatland_status();

/* Window and status modifiers */

//void load_flatland_status(const flat_status&);

}

#endif
