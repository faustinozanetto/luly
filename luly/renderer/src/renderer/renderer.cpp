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

    void renderer::submit_arrays(int count, renderer_draw_mode draw_mode)
    {
        glDrawArrays(get_renderer_draw_mode_to_opengl(draw_mode), 0, count);
    }

    void renderer::submit_vao(const std::shared_ptr<vertex_array_object>& vertex_array_object, int count,
                              renderer_draw_mode draw_mode)
    {
        vertex_array_object->bind();
        submit_arrays(count, draw_mode);
        vertex_array_object->un_bind();
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

    uint32_t renderer::get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode)
    {
        switch (draw_mode)
        {
        case renderer_draw_mode::line_loop:
            return GL_LINE_LOOP;
        case renderer_draw_mode::line_strip:
            return GL_LINE_STRIP;
        case renderer_draw_mode::lines:
            return GL_LINES;
        case renderer_draw_mode::points:
            return GL_POINTS;
        case renderer_draw_mode::triangle_fan:
            return GL_TRIANGLE_FAN;
        case renderer_draw_mode::triangle_strip:
            return GL_TRIANGLE_STRIP;
        case renderer_draw_mode::triangles:
            return GL_TRIANGLES;
        }
        LY_ASSERT_MSG(false, "Unknown renderer draw mode!");
        return -1;
    }
}
