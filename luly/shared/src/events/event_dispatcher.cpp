#include "event_dispatcher.h"

namespace luly::events
{
    event_dispatcher::event_dispatcher(base_event& event) : m_event(event)
    {
    }
}
