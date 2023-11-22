#pragma once

#include "scene/scene_manager.h"


#include <window/window.h>
#include <events/base_event.h>
#include <memory>

#include <renderer_api.h>
#include <imgui/imgui_manager.h>

// Entry point used in applications.
int main(int argc, char** argv);

#define BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace luly::core
{
    class application
    {
    public:
        application(const renderer::window_specification& window_specification);
        virtual ~application();

        /* Getters */
        static application& get() { return *s_instance; }
        std::shared_ptr<renderer::window>& get_window() { return m_window; }
        std::shared_ptr<scene::scene_manager>& get_scene_manager() { return m_scene_manager; }
        std::shared_ptr<renderer::imgui_manager>& get_imgui_manager() { return m_imgui_manager; }

        /* Virtual Methods */
        virtual void on_create();
        virtual void on_update();

        virtual void on_handle_event(events::base_event& event) = 0;

    private:
        void run();
        void on_event(events::base_event& event);

        std::shared_ptr<renderer::window> m_window;
        std::shared_ptr<scene::scene_manager> m_scene_manager;
        std::shared_ptr<renderer::imgui_manager> m_imgui_manager;
        float m_frame_delay;

        friend int ::main(int argc, char** argv);
        static application* s_instance;
    };

    application* create_application();
}
