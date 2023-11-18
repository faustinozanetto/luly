#pragma once

#include "key_event.h"

namespace luly::events
{
    class key_pressed_event : public key_event
    {
    public:
        key_pressed_event(const input::key key_code, bool repeat = false)
            : key_event(key_code), m_repeat(repeat)
        {
        }

        /* Getters */
        bool is_repeat() const { return m_repeat; }

        /* Event */
        static event_type get_static_type() { return event_type::key_pressed; }
        event_type get_type() const override { return event_type::key_pressed; }

    private:
        bool m_repeat;
    };
}
