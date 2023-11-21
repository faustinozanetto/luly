#pragma once

#include "scene/scene_manager.h"

#include "core_api.h"

#include <window/window.h>
#include <events/base_event.h>
#include <memory>

// Entry point used in applications.
int main(int argc, char** argv);

#define BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace luly::core
{
    class LULY_CORE_API application
    {
    public:
        application(const renderer::window_specification& window_specification);
        virtual ~application();

        /* Getters */
        static application& get() { return *s_instance; }
        std::shared_ptr<renderer::window>& get_window() { return m_window; }
        std::shared_ptr<scene::scene_manager>& get_scene_manager() { return m_scene_manager; }

        /* Virtual Methods */
        virtual void on_update() = 0;
        virtual void on_handle_event(events::base_event& event) = 0;

    private:
        void run();
        void on_event(events::base_event& event);

        std::shared_ptr<renderer::window> m_window;
        std::shared_ptr<scene::scene_manager> m_scene_manager;
        float m_frame_delay;

        friend int ::main(int argc, char** argv);
        static application* s_instance;
    };

    application* create_application();
}
