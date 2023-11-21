#pragma once

#include "key_event.h"

#include "shared_api.h"

namespace luly::events
{
    class LULY_SHARED_API key_released_event : public key_event
    {
    public:
        key_released_event(const input::key key_code)
            : key_event(key_code)
        {
        }

        /* Event */
        static event_type get_static_type() { return event_type::key_released; }
        event_type get_type() const override { return event_type::key_released; }
    };
}
