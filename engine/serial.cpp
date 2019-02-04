#include "serial.hpp"
#include "wsdl2/event.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

using namespace wsdl2::event;

template <class T>
inline void emit(event_t * e)
{
    flat::event_channel()->emit(flat::core::signal<T>(flat::core::priority_t::max, T(*e)));
}

void flat::serial::broadcast()
{
    while(auto m_event = poll_event())
    {
        event_t * ev = m_event.get();

        switch (m_event->type())
        {

        // keyboard events
        case SDL_KEYUP:
            emit<e_key>(ev);
            break;
        case SDL_KEYDOWN:
            emit<e_key>(ev);
            break;

        // mouse events
        case SDL_MOUSEMOTION:
            emit<mouse::e_motion>(ev);
            break;
        case SDL_MOUSEBUTTONDOWN:
            emit<mouse::e_button>(ev);
            break;
        case SDL_MOUSEBUTTONUP:
            emit<mouse::e_button>(ev);
            break;
        case SDL_MOUSEWHEEL:
            emit<mouse::e_wheel>(ev);
            break;

        // sdl quit event
        case SDL_QUIT:
            emit<e_quit>(ev);
            break;

        // window events
        case SDL_WINDOWEVENT:

            // from poll_event it's safe to cast without checking
            switch (dynamic_cast<wsdl2::event::window::e_window*>(ev)->action())
            {
            case (wsdl2::event::window::action_t::moved):
                emit<wsdl2::event::window::e_move>(ev);
                break;
            case (wsdl2::event::window::action_t::resized):
                emit<wsdl2::event::window::e_resize>(ev);
                break;
            case (wsdl2::event::window::action_t::size_changed):
                emit<wsdl2::event::window::e_resize>(ev);
                break;
            default:
                emit<wsdl2::event::window::e_window>(ev);
                break;
            }

            break;

        // TODO, other events
        //
        default:
            break;
        }
    }
}

