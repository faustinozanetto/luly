﻿#pragma once

#include "window/window.h"
#include "events/base_event.h"

#include <memory>

// Entry point used in applications.
int main(int argc, char** argv);

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

        /* Virtual Methods */
        virtual void on_update() = 0;
        virtual void on_handle_event(events::base_event& event) = 0;

    private:
        void run();

        std::shared_ptr<renderer::window> m_window;

        friend int ::main(int argc, char** argv);
        static application* s_instance;
    };

    application* create_application();
}
