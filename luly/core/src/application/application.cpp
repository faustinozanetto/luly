#include "lypch.h"
#include "application.h"

#include <time/app_time.h>

#include "renderer/renderer/renderer.h"
#include "renderer/scene/scene_renderer.h"

#include <events/event_dispatcher.h>
#include <utils/assert.h>
#include <logging/log.h>

#include "assets/assets_manager.h"

namespace luly::core
{
    application* application::s_instance = nullptr;

    application::application(const renderer::window_specification& window_specification)
    {
        s_instance = this;
        app_time::update_time();
        
        assets::assets_manager::initialize();

        LY_TRACE("Started creating application...");
        m_window = std::make_shared<renderer::window>(window_specification);
        m_window->set_event_function(BIND_EVENT_FN(application::on_event));

        renderer::renderer::initialize(m_window);
        renderer::scene_renderer::initialize();


        m_scene_manager = std::make_shared<scene::scene_manager>();
        app_time::update_time();

        application::on_create();
    }

    application::~application()
    {
    }

    void application::on_create()
    {
    }

    void application::on_update()
    {
        m_scene_manager->on_update(app_time::get_delta_time());
    }

    void application::run()
    {
        float frame_time = 0;
        while (!renderer::renderer::get_should_close())
        {
            renderer::renderer::clear_screen();

            app_time::update_time();

            if (m_frame_delay > 0.f)
            {
                frame_time -= app_time::get_delta_time();
                if (frame_time <= 0.f)
                {
                    frame_time = m_frame_delay;
                    app_time::set_delta_time(m_frame_delay);
                }
                else
                {
                    continue;
                }
            }

            on_update();

            renderer::renderer::poll_input();
            renderer::renderer::swap_buffers();
        }
    }

    void application::on_event(events::base_event& event)
    {
        on_handle_event(event);
    }
}
