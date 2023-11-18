﻿#pragma once

#include "events/base_event.h"
#include "input/key_codes.h"

namespace luly::events
{
    class key_event : public base_event
    {
    public:
        key_event(const input::key key_code) : m_key_code(key_code)
        {
        }

        /* Getters */
        input::key get_key_code() const { return m_key_code; }

    protected:
        input::key m_key_code;
    };
}
