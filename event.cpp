#include "flatland/event.hpp"

using namespace flat;

event::event(wsdl2::window& refw, core::channel& event_chan) : ref_window(refw) {

    key_listener = event_chan.delegate_task(key_callback);
}

void event::key_callback(wsdl2::event::key e)
{
    if(e.window_id == ref_window.sdl_id()) {
        if (e.type == SDL_KEYDOWN)
            keys_down.insert(e);
        else
            keys_down.erase(e);
    }
}
