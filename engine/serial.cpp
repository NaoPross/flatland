#include "serial.hpp"
#include "wsdl2/event.hpp"
#include "core/signal.hpp"
#include "flatland.hpp"

void flat::serial::broadcast()
{
    while(auto m_event = wsdl2::poll_event())
    {
        using namespace flat::event;

#define EMIT(__type__) event_channel().emit(core::signal<__type__>(__type__(m_event.sdl()), core::priority_t::max);

        switch (m_event.type())
        {

        // keyboard events
        case SDL_KEYUP:
            EMIT(key)
            break;
        case SDL_KEYDOWN:
            EMIT(key)
            break;

        // mouse events
        case SDL_MOUSEMOTION:
            EMIT(mouse)
            break;
        case SDL_MOUSEBUTTONDOWN:
            EMIT(mouse)
            break;
        case SDL_MOUSEBUTTONUP:
            EMIT(mouse)
            break;
        case SDL_MOUSEWHEEL:
            EMIT(mouse)
            break;

        // sdl quit event
        case SDL_QUIT:
            EMIT(quit)
            break;

        // window events
        case SDL_WINDOWEVENT:

            switch (m_event.sdl().window.event)
            {
            case (SDL_WINDOWEVENT_MOVED)
                EMIT(win_move);
                break;
            case (SDL_WINDOWEVENT_RESIZED)
                EMIT(win_resize);
                break;
            case (SDL_WINDOWEVENT_SIZE_CHANGED)
                EMIT(win_resize);
                break;
            default:
                EMIT(window);
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

