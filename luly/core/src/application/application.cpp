#include "lypch.h"
#include "application.h"

#include "engine_ui.h"
#include "events/event_dispatcher.h"
#include "renderer/renderer.h"
#include "time/engine_time.h"

namespace luly::core
{
    application* application::s_instance = nullptr;

    application::application(const renderer::window_specification& window_specification)
    {
        s_instance = this;
        time::update_time();

        LY_TRACE("Started creating application...");
        m_window = std::make_shared<renderer::window>(window_specification);
        m_window->set_event_function(BIND_EVENT_FN(application::on_event));
        renderer::renderer::initialize();
        ui::engine_ui::initialize();

        m_scene_manager = std::make_shared<scene::scene_manager>();
        time::update_time();
    }

    application::~application()
    {
    }

    void application::run()
    {
        float frame_time = 0;
        while (!renderer::renderer::get_should_close())
        {
            renderer::renderer::clear_screen();

            time::update_time();

            if (m_frame_delay > 0.f)
            {
                frame_time -= time::get_delta_time();
                if (frame_time <= 0.f)
                {
                    frame_time = m_frame_delay;
                    time::set_delta_time(m_frame_delay);
                }
                else
                {
                    continue;
                }
            }

            ui::engine_ui::begin_frame();
            on_update();
            ui::engine_ui::on_update();
            ui::engine_ui::end_frame();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }

    void application::on_event(events::base_event& event)
    {
        on_handle_event(event);
    }
}
