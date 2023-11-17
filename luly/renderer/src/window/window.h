#pragma once

#include <lypch.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace luly::renderer
{
    struct window_specification
    {
        std::string title;
        int width;
        int height;
    };

    struct window_data
    {
        int width;
        int height;
        std::string title;
    };

    class window
    {
    public:
        window(const window_specification& window_specification);
        ~window();

    private:
        void initialize();

        GLFWwindow* m_handle;
        window_data m_data;
    };
}
