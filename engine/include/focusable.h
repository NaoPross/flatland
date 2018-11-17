#ifndef __FOCUSABLE_H__
#define __FOCUSABLE_H__

union SDL_Event;

class Focusable
{
    bool focused;

public:

    Focusable(bool focused = true)

        : focused(focused) {}

    virtual void serial_cb(SDL_Event *) = 0;

    void setFocused(bool flag)
    {
        focused = flag;
    }

    bool isFocused() const
    {
        return focused;
    }
};

#endif
