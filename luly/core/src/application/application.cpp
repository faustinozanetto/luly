#include "lypch.h"
#include "application.h"

#include "renderer/renderer/renderer.h"
#include "renderer/scene/scene_renderer.h"
#include "assets/assets_manager.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "physics/physics_world.h"

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
        physics::physics_world::initialize();
        scene::scene_manager::initialize();

        app_time::update_time();
    }

    application::~application()
    {
    }

    void application::on_update()
    {
        LY_PROFILE_FUNCTION;
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        if (!current_scene) return;

        current_scene->get_camera_manager()->get_perspective_camera_controller()->on_update(
            app_time::get_delta_time());

        scene::scene_manager::get().on_update(app_time::get_delta_time());
    }

    void application::run()
    {
        LY_PROFILE_FUNCTION;
        float frame_time = 0;
        while (!renderer::renderer::get_should_close())
        {
            LY_PROFILE_FRAMEMARKER();
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
        LY_PROFILE_FRAMEMARKER();
    }

    void application::on_event(events::base_event& event)
    {
        LY_PROFILE_FUNCTION;
        events::event_dispatcher dispatcher(event);
        dispatcher.dispatch<events::window_resize_event>(BIND_EVENT_FN(on_window_resized_event));

        // Call the on_handle_event for the child classes.
        on_handle_event(event);
    }

    bool application::on_window_resized_event(const events::window_resize_event& window_resize_event)
    {
        LY_PROFILE_FUNCTION;
        renderer::renderer::set_viewport_size(window_resize_event.get_size());
        renderer::scene_renderer::on_resize(window_resize_event.get_size());
        scene::scene_manager::get().on_resize(window_resize_event.get_size());
        return true;
    }
}
