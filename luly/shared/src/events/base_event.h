#pragma once

#include "shared_api.h"

namespace luly::events
{
    enum class LULY_SHARED_API event_type
    {
        window_resize,
        key_pressed,
        key_released,
        mouse_moved,
        mouse_scrolled,
        mouse_button_pressed,
        mouse_button_released
    };

    class LULY_SHARED_API base_event
    {
    public:
        virtual ~base_event() = default;

        /* Getters */
        virtual event_type get_type() const = 0;

        bool m_handled = false;
    };
}
