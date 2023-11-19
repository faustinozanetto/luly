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
        initialize_debug();
        set_clear_color(s_data.clear_color);

        glEnable(GL_DEPTH_TEST);
        glFrontFace(GL_CW);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        
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

    glm::ivec2 renderer::get_viewport_size()
    {
        return glm::ivec2(s_data.window->get_data().width, s_data.window->get_data().height);
    }

    void renderer::submit_arrays(int count, renderer_draw_mode draw_mode)
    {
        glDrawArrays(get_renderer_draw_mode_to_opengl(draw_mode), 0, count);
    }

    void renderer::submit_arrays_instanced(int count, int instance_count, renderer_draw_mode draw_mode)
    {
        glDrawArraysInstanced(get_renderer_draw_mode_to_opengl(draw_mode), 0, count, instance_count);
    }

    void renderer::submit_elements(int count, renderer_draw_mode draw_mode)
    {
        glDrawElements(get_renderer_draw_mode_to_opengl(draw_mode), count, GL_UNSIGNED_INT, 0);
    }

    void renderer::submit_vao(const std::shared_ptr<vertex_array_object>& vao, int count, renderer_draw_mode draw_mode)
    {
        vao->bind();
        submit_arrays(count, draw_mode);
        vao->un_bind();
    }

    void renderer::submit_vao_instanced(const std::shared_ptr<vertex_array_object>& vao,
                                        int count, int instance_count, renderer_draw_mode draw_mode)
    {
        vao->bind();
        submit_arrays_instanced(count, instance_count, draw_mode);
        vao->un_bind();
    }

    void renderer::submit_mesh(const std::shared_ptr<mesh>& mesh, renderer_draw_mode draw_mode)
    {
        mesh->get_vao()->bind();
        submit_elements(mesh->get_vao()->get_element_buffer()->get_count(), draw_mode);
        mesh->get_vao()->un_bind();
    }

    void renderer::submit_model(const std::shared_ptr<model>& model, renderer_draw_mode draw_mode)
    {
        for (const auto& mesh : model->get_meshes())
        {
            submit_mesh(mesh, draw_mode);
        }
    }

    void renderer::submit_model_instanced(const std::shared_ptr<model>& model,
                                          int instance_count, renderer_draw_mode draw_mode)
    {
        for (const auto& mesh : model->get_meshes())
        {
            submit_vao_instanced(mesh->get_vao(), mesh->get_vao()->get_element_buffer()->get_count(),
                                 instance_count, draw_mode);
        }
    }

    void renderer::bind_texture(int slot, uint32_t handle)
    {
        glBindTextureUnit(slot, handle);
    }

    void renderer::initialize_data()
    {
        core::application& application = core::application::get();
        s_data.window = application.get_window();
        s_data.clear_color = glm::vec4(0.85f);
    }

    void renderer::initialize_glad()
    {
        const int glad_initialize_status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        LY_ASSERT_MSG(glad_initialize_status, "Failed to initialize glad!")
        LY_TRACE("Glad initialized successfully!");

        auto renderer = (const char*)glGetString(GL_RENDERER);
        auto version = (const char*)glGetString(GL_VERSION);
        auto vendor = (const char*)glGetString(GL_VENDOR);
        auto glsl_version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

        LY_TRACE("OpenGL information:");
        LY_TRACE(" - Renderer: {0}", renderer);
        LY_TRACE(" - OpenGL version supported: {0}", version);
        LY_TRACE(" - Vendor: {0}", vendor);
        LY_TRACE(" - GLSL Version: {0}", glsl_version);
    }

    void renderer::initialize_debug()
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(renderer::open_gl_message_callback, nullptr);
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

    void renderer::open_gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH: {
            LY_CRITICAL("OpenGL Debug Message (High Severity)");
            break;
        }
        case GL_DEBUG_SEVERITY_MEDIUM: {
            LY_WARN("OpenGL Debug Message (Medium Severity)");
            break;
        }
        case GL_DEBUG_SEVERITY_LOW: {
            LY_INFO("OpenGL Debug Message (Low Severity)");
            break;
        }
        case GL_DEBUG_SEVERITY_NOTIFICATION: {
            LY_TRACE("OpenGL Debug Message (Notification)");
            break;
        }
        default: {
            LY_ERROR("Unknown OpenGL Debug Message Severity");
            break;
        }
        }

        LY_INFO("   - Source: 0x{:X}", source);
        LY_INFO("   - Type: 0x{:X}", type);
        LY_INFO("   - ID: {}", id);
        LY_INFO("   - Severity: 0x{:X}", severity);
        LY_INFO("   - Message Length: {}", length);
        LY_INFO("   - Message: {}", message);
    }
}
