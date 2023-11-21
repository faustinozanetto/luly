#pragma once

#include "events/base_event.h"
#include "input/mouse_codes.h"

#include "shared_api.h"

namespace luly::events
{
    class LULY_SHARED_API mouse_button_event : public base_event
    {
    public:
        mouse_button_event(const input::mouse_button button_code)
            : m_button_code(button_code)
        {
        }

        /* Getters */
        input::mouse_button get_button_code() const { return m_button_code; }

    protected:
        input::mouse_button m_button_code;
    };
}
