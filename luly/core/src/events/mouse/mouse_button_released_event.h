#pragma once

#include "mouse_button_event.h"

namespace luly::events
{
    class mouse_button_released_event : public mouse_button_event
    {
    public:
        mouse_button_released_event(const input::mouse_button button_code) : mouse_button_event(button_code)
        {
        }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_button_released; }
        event_type get_type() const override { return event_type::mouse_button_released; }
    };
}
