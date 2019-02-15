#ifndef __FLATLAND_H__
#define __FLATLAND_H__

#include <string>
#include <vector>
#include <functional>
#include "window.hpp"

namespace wsdl2 {

    class renderer;
}

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
 *
 * init: get a new window instance in lambda, 
 *       it'll be erased exiting the loop function
 *
 * loop: start a new loop
 */

bool init(const std::function<window*()>& f = [](){ return new window("Default", 860, 640); });
int loop(float fps = 60);

void quit(int code = 0);
void force_quit(int code, const std::string& reason = "");

/* 
 * Engine channels 
 */
core::channel& core_channel();

// Main job access
core::job& main_job();

// public window access
window * main_window();

// Renderer public access
wsdl2::renderer * renderer();

// TODO, accessors to renderbases
renderbase * find_renderable(std::size_t uuid);

std::vector<renderbase*> find_renderable(const std::string& label);

}

#endif
