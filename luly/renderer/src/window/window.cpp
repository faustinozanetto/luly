#include "window.h"

#include "events/window/window_resize_event.h"

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

    void window::setup_glfw_callbacks()
    {
        // 1. Resize callback
        glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* native_window, int width, int height)
        {
            window_data& data = *static_cast<window_data*>(glfwGetWindowUserPointer(native_window));
            data.width = width;
            data.height = height;

            // Dispatch event.
            events::window_resize_event event({width, height});
            data.event_func(event);
        });
    }

    void window::make_current()
    {
        glfwMakeContextCurrent(m_handle);
    }

    void window::set_event_function(const std::function<void(events::base_event&)> func)
    {
        m_data.event_func = func;
    }
}
