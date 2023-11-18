#pragma once

#include "events/base_event.h"

namespace luly::events
{
    class mouse_moved_event : public base_event
    {
    public:
        mouse_moved_event(float x, float y) : m_pos_x(x), m_pos_y(y)
        {
        }

        /* Getters */
        float get_mouse_x() const { return m_pos_x; }
        float get_mouse_y() const { return m_pos_y; }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_moved; }
        event_type get_type() const override { return event_type::mouse_moved; }

    private:
        float m_pos_x;
        float m_pos_y;
    };
}
