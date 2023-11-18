#pragma once

#include "events/base_event.h"

#include <functional>
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
        std::function<void(events::base_event&)> event_func;
    };

    class window
    {
    public:
        window(const window_specification& window_specification);
        ~window();

        /* Getters */
        GLFWwindow* get_native_handle() const { return m_handle; }
        const window_data& get_data() const { return m_data; }

        /* Setters */
        void set_event_function(std::function<void(events::base_event&)> func);

    private:
        void initialize();
        void initialize_glfw();
        void setup_glfw_hints();
        void create_glfw_handle();
        void setup_glfw_callbacks();
        void make_current();

        GLFWwindow* m_handle;
        window_data m_data;
    };
}
