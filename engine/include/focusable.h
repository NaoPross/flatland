#ifndef __FOCUSABLE_H__
#define __FOCUSABLE_H__

#include <vector>

union SDL_Event;
class task_s;

struct SDL_EventCollector
{
    task_s * checker;
    std::vector<SDL_Event> stack;

    SDL_EventCollector();
    ~SDL_EventCollector();

    void collect(void*);
};

class Focusable
{
    bool focused;

    task_s * event_trigger;

    static SDL_EventCollector * collector;

public:

    Focusable(bool focused = true);

    virtual ~Focusable();

    virtual void serial_cb(const SDL_Event&) = 0;

    void setFocused(bool flag);

    bool isFocused() const;

    void serial_precall(void*);
};

#endif
