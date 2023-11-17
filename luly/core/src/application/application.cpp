#include "lypch.h"
#include "application.h"

#include "renderer/renderer.h"

namespace luly::core
{
    application* application::s_instance = nullptr;

    application::application(const renderer::window_specification& window_specification)
    {
        s_instance = this;

        LY_INFO("Started creating application...");
        m_window = std::make_shared<renderer::window>(window_specification);

        renderer::renderer::initialize();
    }

    application::~application()
    {
    }

    void application::run()
    {
        while (!renderer::renderer::get_should_close())
        {
            renderer::renderer::clear_screen();


            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }
}
