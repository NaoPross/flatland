#pragma once

#include <set>
#include <wsdl2/event.hpp>
#include "flatland/core/signal.hpp"

namespace flat {

    class event 
    {
    public:
        event(wsdl2::window&, core::channel&);

        void is_keydown(wsdl2::event::key::keycode) const;

    private:
        // reference focused window
        wsdl2::window& ref_window;

        void key_callback(wsdl2::event::key);

        // find if a key is present with 
        // a logaritmic temporal complexity O(log(keys_down.size()))
        std::set<wsdl2::event::key::keycode> keys_down;

        // listener related to the keyboard events
        std::shared_ptr<core::listener<void, wsdl2::event::key>> key_listener;
    };
};
