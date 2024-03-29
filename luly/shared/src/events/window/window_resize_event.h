﻿#pragma once

#include "events/base_event.h"
#include "shared_api.h"

#include <glm/glm.hpp>

namespace luly::events
{
    class LULY_SHARED_API window_resize_event : public base_event
    {
    public:
        window_resize_event(const glm::ivec2& window_size) : m_window_size(window_size)
        {
        }

        /* Getters */
        glm::ivec2 get_size() const { return m_window_size; }

        static event_type get_static_type() { return event_type::window_resize; }
        event_type get_type() const override { return event_type::window_resize; }

    private:
        glm::ivec2 m_window_size;
    };
}
