#pragma once

#include "scene/scene_manager.h"

#include "renderer/window/window.h"

#include <events/base_event.h>
#include <memory>

#include "events/window/window_resize_event.h"

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
        const std::string& get_initial_title() const { return m_initial_title; }

        /* Virtual Methods */
        virtual void on_create() = 0;
        virtual void on_update();

        virtual void on_handle_event(events::base_event& event) = 0;

    private:
        void run();
        void on_event(events::base_event& event);
        bool on_window_resized_event(const events::window_resize_event& window_resize_event);

        std::shared_ptr<renderer::window> m_window;
        std::string m_initial_title;
        float m_frame_delay;

        friend int ::main(int argc, char** argv);
        static application* s_instance;
    };

    application* create_application();
}
