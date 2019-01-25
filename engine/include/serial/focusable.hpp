#ifndef __FOCUSABLE_H__
#define __FOCUSABLE_H__

#include "object.hpp"
#include "types.hpp"

union SDL_Event;

namespace flat {

class Focusable : virtual public flat::object
{
    bool focused;

protected:

    /* Callback to event */
    virtual void serial_cb(const SDL_Event*) = 0;

    /* Event stack specification */
    virtual uint32_t stackID() const = 0;

public:

    Focusable(bool focused = true);

    virtual ~Focusable();

    void setFocused(bool flag);

    bool isFocused() const;

    void serial_precall(void*);
};

}

#endif
