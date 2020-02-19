#pragma once

#include "flatland/core/signal.hpp"
#include "flatland/trait/bounded.hpp"

namespace flat {

    /*
    * Abstract collision detector class
    * Notice that this class is a channel, then 
    * it needs a broadcaster job to be initialized
    */
    class coll_detc : private flat::core::channel {
    protected:
        
        // send signal when a detection is found
        void overlap_event();
        
    public: 

        using channel::channel;
    
        // TODO, allow flat::collision only
        using channel::connect;

        // insert/erase an object into the structure, so that it could be checked
        virtual void insert(const flat::trait::bounded&) = 0;
        virtual void erase(const flat::trait::bounded&) = 0;
        
        // check whether collisions are enabled
        virtual bool coll_enabled() const = 0;

        // set collision check state
        // basically it's not supposed to do something, but it depends on the sub-implementation of this class
        virtual void set_coll_enabled(bool) {}
    };
};
