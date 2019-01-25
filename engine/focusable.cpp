#include "serial/focusable.hpp"
#include "core/task.hpp"
#include "serial.hpp"

Focusable::Focusable(bool focused) : focused(focused) 
{
    event_trigger = new flat::core::task<Focusable>(this, &Focusable::serial_precall, 0); 
}

Focusable::~Focusable()
{
    delete event_trigger;
}

void Focusable::setFocused(bool flag)
{
    focused = flag;
}

bool Focusable::isFocused() const
{
    return focused;
}

void Focusable::serial_precall(void*)
{
    for (auto event : FlatSerial::collector->getStack(stackID()))
        serial_cb(&event);
}

