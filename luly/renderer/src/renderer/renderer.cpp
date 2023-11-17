#include "renderer.h"

#include <application/application.h>
#include <utils/assert.h>
#include <logging/log.h>

namespace luly::renderer
{
    static renderer_data s_data;

    void renderer::initialize()
    {
        LY_TRACE("Started initializing renderer...");
        initialize_data();
        initialize_glad();

        LY_TRACE("Renderer initialized successfully!");
    }

    void renderer::swap_buffers()
    {
        glfwSwapBuffers(s_data.window->get_native_handle());
    }

    void renderer::poll_input()
    {
        glfwPollEvents();
    }

    bool renderer::get_should_close()
    {
        return glfwWindowShouldClose(s_data.window->get_native_handle());
    }

    void renderer::clear_screen()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void renderer::set_clear_color(const glm::vec4& clear_color)
    {
        s_data.clear_color = clear_color;
        glClearColor(s_data.clear_color.r, s_data.clear_color.g, s_data.clear_color.b, s_data.clear_color.a);
    }

    void renderer::initialize_data()
    {
        core::application& application = core::application::get();
        s_data.window = application.get_window();
    }

    void renderer::initialize_glad()
    {
        const int glad_initialize_status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        LY_ASSERT_MSG(glad_initialize_status, "Failed to initialize glad!")
        LY_TRACE("Glad initialized successfully!");
    }
}
