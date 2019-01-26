#ifndef __FLATLAND_H__
#define __FLATLAND_H__

namespace flat {

class FlatWindow;

struct flat_status
{

    flat_status(unsigned char video = 1,
                unsigned char audio = 1,
                unsigned char timer = 1,
                unsigned char events = 1,
                unsigned char joystick = 0,
                unsigned char controller = 0,
                unsigned char haptic = 0,
                unsigned char error = 0,
                unsigned char running = 0,
                unsigned char loop = 0)

        : video(video), audio(audio), timer(timer), events(events),
          joystick(joystick), controller(controller), haptic(haptic),
          error(error), running(running), loop(loop) {}

    unsigned char video:1;
    unsigned char audio:1;
    unsigned char timer:1;
    unsigned char events:1;
    unsigned char joystick:1;
    unsigned char controller:1;
    unsigned char haptic:1;
    unsigned char error:1;
    unsigned char running:1;
    unsigned char loop:1;
};

int init_flatland(FlatWindow*, const flat_status&, float fps = 60);
void quit();

namespace core {
    
    class job;
    class task;
    class channel;
}

/* Engine channels */

core::channel& core_channel();

/* Main job access */

core::job& main_job();

/* Window and status accessors */

FlatWindow * getFlatWindow();

flat_status flatland_status();

/* Window and status modifiers */

//void load_flatland_status(const flat_status&);

}

#endif
