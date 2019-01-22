#ifndef __FOCUSABLE_H__
#define __FOCUSABLE_H__

#include "object.h"
#include "types.h"

class task_s;
union SDL_Event;

class Focusable : virtual public flat::core::object
{
    bool focused;

    task_s * event_trigger;

protected:

    /* Callback to event */
    virtual void serial_cb(const SDL_Event*) = 0;

    /* Event stack specification */
    virtual Uint32 stackID() const = 0;

public:

    Focusable(bool focused = true);

    virtual ~Focusable();

    void setFocused(bool flag);

    bool isFocused() const;

    void serial_precall(void*);
};

#endif
