﻿#pragma once

#include "base_event.h"
#include "shared_api.h"

#include <functional>

namespace luly::events
{
    class LULY_SHARED_API event_dispatcher
    {
    public:
        event_dispatcher(base_event& event);

        template <typename T>
        bool dispatch(std::function<bool(T&)> func)
        {
            if (m_event.get_type() == T::get_static_type())
            {
                m_event.m_handled = func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        base_event& m_event;
    };
}
