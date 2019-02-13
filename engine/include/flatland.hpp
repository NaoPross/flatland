#ifndef __FLATLAND_H__
#define __FLATLAND_H__

#include <string>
#include <vector>

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

namespace wsdl2
{
    class renderer;
}

class renderbase;

/*
 * Initialization
 */

bool init();
int loop(window&, float fps = 60);

void quit(int code = 0);
void force_quit(int code, const std::string& reason = "");

/* 
 * Engine channels 
 */
core::channel& core_channel();

// Main job access
core::job& main_job();

// Renderer public access
wsdl2::renderer * renderer();

// TODO, accessors to renderbases
renderbase * find_renderable(std::size_t uuid);

std::vector<renderbase*> find_renderable(const std::string& label);

}

#endif
