#ifndef __KEYFOCUSABLE_H__
#define __KEYFOCUSABLE_H__

#include "focusable.hpp"

struct SDL_KeyboardEvent;

namespace flat {

class KeyFocusable : virtual public Focusable
{

    virtual void serial_cb(const SDL_Event*) override;

    virtual uint32_t stackID() const override;

protected:

    virtual void key_cb(const SDL_KeyboardEvent*) = 0;
};

}

#endif
