﻿#pragma once

#include "window/window.h"
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

        static application& get() { return *s_instance; }

    private:
        void run();

        std::shared_ptr<renderer::window> m_window;

        friend int ::main(int argc, char** argv);
        static application* s_instance;
    };

    application* create_application();
}
