#include "serial/focusable.hpp"
#include "core/task.hpp"
#include "serial.hpp"

using namespace flat;

Focusable::Focusable(bool focused) : m_focused(focused) 
{
    // event_trigger = new flat::core::task<Focusable>(this, &Focusable::serial_precall, 0); 
}

Focusable::~Focusable()
{
    // delete event_trigger;
}

void Focusable::setFocused(bool flag)
{
    m_focused = flag;
}

bool Focusable::isFocused() const
{
    return m_focused;
}

void Focusable::serial_precall(void*)
{
    for (auto event : FlatSerial::collector->getStack(stackID()))
        serial_cb(&event);
}

