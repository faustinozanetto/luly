#include "lypch.h"
#include "application.h"

namespace luly::core
{
    application* application::s_instance = nullptr;

    application::application(const renderer::window_specification& window_specification)
    {
        s_instance = this;

        LY_INFO("Started creating application...");
        m_window = std::make_shared<renderer::window>(window_specification);
    }

    application::~application()
    {
    }

    void application::run()
    {
    }
}
