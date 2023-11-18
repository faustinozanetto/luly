#pragma once

#include "events/base_event.h"

namespace luly::events
{
    class mouse_scrolled_event : public base_event
    {
    public:
        mouse_scrolled_event(float x_offset, float y_offset) : m_offset_x(x_offset), m_offset_y(y_offset)
        {
        }

        /* Getters */
        float get_offset_x() const { return m_offset_x; }
        float get_offset_y() const { return m_offset_y; }

        /* Event */
        static event_type get_static_type() { return event_type::mouse_scrolled; }
        virtual event_type get_type() const override { return event_type::mouse_scrolled; }

    private:
        float m_offset_x;
        float m_offset_y;
    };
}
