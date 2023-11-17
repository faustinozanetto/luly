#pragma once

#include "window/window.h"

#include <luly.h>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace luly::renderer
{
    struct renderer_data
    {
        std::shared_ptr<window> window;
        glm::vec4 clear_color;
    };

    class renderer
    {
    public:
        static void initialize();

        static void swap_buffers();
        static void poll_input();
        static bool get_should_close();
        static void clear_screen();
        static void set_clear_color(const glm::vec4& clear_color);

    private:
        static void initialize_data();
        static void initialize_glad();
    };
}
