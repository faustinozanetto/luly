#include "lypch.h"
#include "application.h"

#include "engine_ui.h"
#include "events/event_dispatcher.h"
#include "renderer/renderer.h"

namespace luly::core
{
    application* application::s_instance = nullptr;

    application::application(const renderer::window_specification& window_specification)
    {
        s_instance = this;

        LY_TRACE("Started creating application...");
        m_window = std::make_shared<renderer::window>(window_specification);
        renderer::renderer::initialize();
        ui::engine_ui::initialize();

        m_scene_manager = std::make_shared<scene::scene_manager>();
    }

    application::~application()
    {
    }

    void application::run()
    {
        while (!renderer::renderer::get_should_close())
        {
            renderer::renderer::clear_screen();

            ui::engine_ui::begin_frame();
            on_update();
            ui::engine_ui::on_update();
            ui::engine_ui::end_frame();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }
}
