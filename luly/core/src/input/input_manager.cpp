#include "lypch.h"

#include <input/input_manager.h>

#include "application/application.h"

namespace luly::input
{
    bool input_manager::is_key_pressed(key key_code)
    {
        GLFWwindow* window = core::application::get().get_window()->get_native_handle();
        const int state = glfwGetKey(window, static_cast<int>(key_code));
        return state == GLFW_PRESS;
    }

    bool input_manager::is_mouse_button_pressed(mouse_button button_code)
    {
        GLFWwindow* window = core::application::get().get_window()->get_native_handle();
        const int state = glfwGetMouseButton(window, static_cast<int>(button_code));
        return state == GLFW_PRESS;
    }

    glm::vec2 input_manager::get_mouse_position()
    {
        GLFWwindow* window = core::application::get().get_window()->get_native_handle();
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        return {static_cast<float>(x_pos), static_cast<float>(y_pos)};
    }
}
