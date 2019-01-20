#ifndef __KEYFOCUSABLE_H__
#define __KEYFOCUSABLE_H__

#include "focusable.h"

struct SDL_KeyboardEvent;

class KeyFocusable : virtual public Focusable
{

    virtual void serial_cb(const SDL_Event*) override;

    virtual Uint32 stackID() const override;

protected:

    virtual void key_cb(const SDL_KeyboardEvent*) = 0;

public: 

    using Focusable::Focusable;
};

#endif
