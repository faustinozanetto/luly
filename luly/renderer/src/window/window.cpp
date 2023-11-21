#include "window.h"

#include <shared_api.h>

#include <events/window/window_resize_event.h>
#include <events/key/key_pressed_event.h>
#include <events/key/key_released_event.h>
#include <events/mouse/mouse_button_pressed_event.h>
#include <events/mouse/mouse_button_released_event.h>
#include <events/mouse/mouse_moved_event.h>
#include <events/mouse/mouse_scrolled_event.h>

#include <logging/log.h>
#include <utils/assert.h>

namespace luly::renderer
{
    window::window(const window_specification& window_specification)
    {
        m_data.title = window_specification.title;
        m_data.width = window_specification.width;
        m_data.height = window_specification.height;

        initialize();
    }

    window::~window()
    {
    }

    void window::initialize()
    {
        LY_TRACE("Started initializing window...");

        initialize_glfw();
        setup_glfw_hints();
        create_glfw_handle();

        glfwSetWindowUserPointer(m_handle, &m_data);

        setup_glfw_callbacks();
        make_current();
        LY_TRACE("Window initialized successfully!");
    }

    void window::initialize_glfw()
    {
        const int glfw_initialize_status = glfwInit();
        LY_ASSERT_MSG(glfw_initialize_status != GLFW_FALSE, "Failed to initialize GFLW!")
    }

    void window::setup_glfw_hints()
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    }

    void window::create_glfw_handle()
    {
        LY_TRACE("Creating GLFW window: ");
        LY_TRACE("   - Title: " + m_data.title);
        LY_TRACE(std::format("   - Width: {} px", m_data.width));
        LY_TRACE(std::format("   - Height: {} px", m_data.height));

        m_handle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
        LY_ASSERT_MSG(m_handle != nullptr, "Failed to create GLFW window!");

        LY_TRACE("GLFW window created successfully!");
    }

    void window::setup_glfw_callbacks() const
    {
        // 1. Resize callback
        glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* native_window, int width, int height)
        {
            window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(native_window));
            data.width = width;
            data.height = height;

            events::window_resize_event event({width, height});
            data.event_func(event);
        });

        // 2. Key callback
        glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                {
                    events::key_pressed_event event(static_cast<input::key>(key), false);
                    data.event_func(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    events::key_released_event event(static_cast<input::key>(key));
                    data.event_func(event);
                    break;
                }
            case GLFW_REPEAT:
                {
                    events::key_pressed_event event(static_cast<input::key>(key), true);
                    data.event_func(event);
                    break;
                }
            }
        });

        // 3. Mouse button callback
        glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods)
        {
            const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

            switch (action)
            {
            case GLFW_PRESS:
                {
                    events::mouse_button_pressed_event event(static_cast<input::mouse_button>(button));
                    data.event_func(event);
                    break;
                }
            case GLFW_RELEASE:
                {
                    events::mouse_button_released_event event(static_cast<input::mouse_button>(button));
                    data.event_func(event);
                    break;
                }
            }
        });

        // 4. Mouse scroll callback
        glfwSetScrollCallback(m_handle, [](GLFWwindow* window, double x_offset, double y_offset)
        {
            const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

            events::mouse_scrolled_event event(static_cast<float>(x_offset), static_cast<float>(y_offset));
            data.event_func(event);
        });

        // 5. Mouse move callback
        glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double x_pos, double y_pos)
        {
            const window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(window));

            events::mouse_moved_event event(static_cast<float>(x_pos), static_cast<float>(y_pos));
            data.event_func(event);
        });
    }

    void window::make_current() const
    {
        glfwMakeContextCurrent(m_handle);
    }

    void window::set_event_function(const std::function<void(events::base_event&)>& func)
    {
        m_data.event_func = func;
    }
}
