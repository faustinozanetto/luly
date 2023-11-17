#pragma once

#include <string>

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

        /* Getters */
        const GLFWwindow* get_native_handle() const { return m_handle; }
        const window_data& get_data() const { return m_data; }

    private:
        void initialize();
        void initialize_glfw();
        void setup_glfw_hints();
        void create_glfw_handle();
        void setup_glfw_callbacks();

        GLFWwindow* m_handle;
        window_data m_data;
    };
}
