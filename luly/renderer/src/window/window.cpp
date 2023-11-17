#include "window.h"

namespace luly::renderer
{
    window::window(const window_specification& window_specification)
    {
        LY_TRACE("Window Specification:");
        LY_TRACE("   - Title: {0}", window_specification.title);
        LY_TRACE("   - Width: {0}", window_specification.width);
        LY_TRACE("   - Height: {0}", window_specification.height);

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
        LY_TRACE("Started initializaing window...");
        const int glfw_initialize_status = glfwInit();
        LY_ASSERT_MSG(glfw_initialize_status != GLFW_FALSE, "Failed to initialize GFLW!");

        // GLFW window hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

        // Window handle creation
        m_handle = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);

        // Store data pointer
        glfwSetWindowUserPointer(m_handle, &m_data);
        LY_TRACE("Window initialized successfully!");
    }
}
