#pragma once

#include "flatland/core/signal.hpp"
#include "flatland/trait/bounded.hpp"

namespace flat {

    /*
    * Abstract collision detector class
    * Notice that this class is a channel, then 
    * it needs a broadcaster jaob to be initialized
    */
    class coll_detc : private flat::core::channel {
    protected:
        
        // send overlap signal
        void overlap_event();
        
    public: 

        using channel::channel;
    
        // TODO, allow flat::collision only
        using channel::connect;
        
        // check whether collisions are enabled
        virtual bool coll_enabled() const = 0;

        // set collision check state
        virtual void set_coll_enabled(bool) {}
    };
};
