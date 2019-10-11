#include "flatland/event.hpp"

using namespace flat;

event::event(wsdl2::window& refw, core::channel& event_chan, core::job& update) 
    : ref_window(refw), on_key_down(update), on_key_up(update) {

    key_listener = event_chan.connect(key_callback);
}

void event::key_callback(wsdl2::event::key e)
{
    if(e.window_id == ref_window.sdl_id()) {
        if (e.type == SDL_KEYDOWN)
            keys_down.insert(e.code());
            on_key_down.emit(e.code());
        else
            keys_down.erase(e.code());
            on_key_up.emit(e.code());
    }
}
