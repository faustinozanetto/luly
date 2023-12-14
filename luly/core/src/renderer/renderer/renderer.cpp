#include "lypch.h"
#include "renderer.h"

#include "renderer/textures/texture_utils.h"

namespace luly::renderer
{
    renderer_data renderer::s_data = {};

    void renderer::initialize(const std::shared_ptr<window>& window)
    {
        LY_PROFILE_FUNCTION;
        LY_TRACE("Started initializing renderer...");
        s_data.window = window;

        initialize_glad();
        initialize_data();
        initialize_debug();
        set_clear_color(s_data.clear_color);

        LY_PROFILE_CONTEXT;
        LY_TRACE("Renderer initialized successfully!");
    }

    void renderer::swap_buffers()
    {
        LY_PROFILE_FUNCTION;
        glfwSwapBuffers(s_data.window->get_native_handle());
        LY_PROFILE_COLLECT;
    }

    void renderer::poll_input()
    {
        LY_PROFILE_FUNCTION;
        glfwPollEvents();
    }

    bool renderer::get_should_close()
    {
        LY_PROFILE_FUNCTION;
        return glfwWindowShouldClose(s_data.window->get_native_handle());
    }

    void renderer::clear_screen()
    {
        LY_PROFILE_FUNCTION;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void renderer::set_clear_color(const glm::vec4& clear_color)
    {
        LY_PROFILE_FUNCTION;
        s_data.clear_color = clear_color;
        glClearColor(s_data.clear_color.r, s_data.clear_color.g, s_data.clear_color.b, s_data.clear_color.a);
    }

    void renderer::set_state(renderer_state state, bool is_enabled)
    {
        LY_PROFILE_FUNCTION;
        if (is_enabled)
        {
            glEnable(get_renderer_state_to_opengl(state));
            return;
        }
        glDisable(get_renderer_state_to_opengl(state));
    }

    void renderer::set_depth_func(renderer_depth_func depth_func)
    {
        LY_PROFILE_FUNCTION;
        glDepthFunc(get_renderer_depth_func_to_opengl(depth_func));
    }

    void renderer::set_cull_face_mode(renderer_cull_face_mode cull_face_mode)
    {
        LY_PROFILE_FUNCTION;
        glCullFace(get_renderer_cull_face_mode_to_opengl(cull_face_mode));
    }

    glm::ivec2 renderer::get_viewport_size()
    {
        LY_PROFILE_FUNCTION;
        return {s_data.window->get_data().width, s_data.window->get_data().height};
    }

    bool renderer::get_is_vsync_enabled()
    {
        LY_PROFILE_FUNCTION;
        return s_data.vsync_enabled;
    }

    void renderer::set_vsync_enabled(bool vsync_enabled)
    {
        LY_PROFILE_FUNCTION;
        s_data.vsync_enabled = vsync_enabled;
        glfwSwapInterval(vsync_enabled ? 1 : 0);
    }

    void renderer::set_viewport_size(const glm::ivec2& viewport_size)
    {
        LY_PROFILE_FUNCTION;
        glViewport(0, 0, viewport_size.x, viewport_size.y);
    }

    void renderer::submit_arrays(int count, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        glDrawArrays(get_renderer_draw_mode_to_opengl(draw_mode), 0, count);
    }

    void renderer::submit_arrays_instanced(int count, int instance_count, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        glDrawArraysInstanced(get_renderer_draw_mode_to_opengl(draw_mode), 0, count, instance_count);
    }

    void renderer::submit_elements(int count, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        glDrawElements(get_renderer_draw_mode_to_opengl(draw_mode), count, GL_UNSIGNED_INT, 0);
    }

    void renderer::submit_vao(const std::shared_ptr<vertex_array_object>& vao, int count, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        vao->bind();
        submit_arrays(count, draw_mode);
        vao->un_bind();
    }

    void renderer::submit_vao_instanced(const std::shared_ptr<vertex_array_object>& vao,
                                        int count, int instance_count, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        vao->bind();
        submit_arrays_instanced(count, instance_count, draw_mode);
        vao->un_bind();
    }

    void renderer::submit_mesh(const std::shared_ptr<mesh>& mesh, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        mesh->get_vao()->bind();
        submit_elements(mesh->get_vao()->get_element_buffer()->get_count(), draw_mode);
        mesh->get_vao()->un_bind();
    }

    void renderer::submit_model(const std::shared_ptr<model>& model, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        for (const std::shared_ptr<mesh>& mesh : model->get_meshes())
        {
            submit_mesh(mesh, draw_mode);
        }
    }

    void renderer::submit_model_instanced(const std::shared_ptr<model>& model,
                                          int instance_count, renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
        for (const auto& mesh : model->get_meshes())
        {
            submit_vao_instanced(mesh->get_vao(), mesh->get_vao()->get_element_buffer()->get_count(),
                                 instance_count, draw_mode);
        }
    }

    void renderer::bind_texture(int slot, uint32_t handle)
    {
        LY_PROFILE_FUNCTION;
        glBindTextureUnit(slot, handle);
    }

    void renderer::blit_frame_buffer(const glm::ivec2& source_position, const glm::ivec2& source_dimensions,
                                     const glm::ivec2& target_position, const glm::ivec2& target_dimensions,
                                     renderer_bit_mask mask, texture_filtering filter)
    {
        LY_PROFILE_FUNCTION;
        glBlitFramebuffer(source_position.x, source_position.y, source_dimensions.x, source_dimensions.y,
                          target_position.x,
                          target_position.y, target_dimensions.x, target_dimensions.y,
                          static_cast<uint32_t>(mask),
                          texture_utils::get_texture_filtering_to_opengl(filter));
    }

    void renderer::initialize_glad()
    {
        LY_PROFILE_FUNCTION;
        const int glad_initialize_status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        LY_ASSERT_MSG(glad_initialize_status, "Failed to initialize glad!")
        LY_TRACE("Glad initialized successfully!");

        auto renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));
        auto vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        auto glsl_version = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

        LY_TRACE("OpenGL information:");
        LY_TRACE(" - Renderer: {0}", renderer);
        LY_TRACE(" - OpenGL version supported: {0}", version);
        LY_TRACE(" - Vendor: {0}", vendor);
        LY_TRACE(" - GLSL Version: {0}", glsl_version);
    }

    void renderer::initialize_debug()
    {
        LY_PROFILE_FUNCTION;
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        //   glDebugMessageCallback(renderer::open_gl_message_callback, nullptr);
    }

    uint32_t renderer::get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode)
    {
        LY_PROFILE_FUNCTION;
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

    uint32_t renderer::get_renderer_state_to_opengl(renderer_state state)
    {
        LY_PROFILE_FUNCTION;
        switch (state)
        {
        case renderer_state::depth:
            return GL_DEPTH_TEST;
        case renderer_state::blend:
            return GL_BLEND;
        case renderer_state::face_cull:
            return GL_CULL_FACE;
        }
        LY_ASSERT_MSG(false, "Unknown renderer state!");
        return -1;
    }

    uint32_t renderer::get_renderer_depth_func_to_opengl(renderer_depth_func depth_func)
    {
        LY_PROFILE_FUNCTION;
        switch (depth_func)
        {
        case renderer_depth_func::less:
            return GL_LESS;
        case renderer_depth_func::less_or_equal:
            return GL_LEQUAL;
        case renderer_depth_func::equal:
            return GL_EQUAL;
        case renderer_depth_func::greater:
            return GL_GREATER;
        case renderer_depth_func::greater_or_equal:
            return GL_GEQUAL;
        case renderer_depth_func::not_equal:
            return GL_NOTEQUAL;
        case renderer_depth_func::always:
            return GL_ALWAYS;
        case renderer_depth_func::never:
            return GL_NEVER;
        default:
            return GL_LESS;
        }
    }

    uint32_t renderer::get_renderer_bit_mask_to_opengl(renderer_bit_mask bit_mask)
    {
        LY_PROFILE_FUNCTION;
        switch (bit_mask)
        {
        case renderer_bit_mask::depth:
            return GL_DEPTH_BUFFER_BIT;
        case renderer_bit_mask::stencil:
            return GL_STENCIL_BUFFER_BIT;
        case renderer_bit_mask::color:
            return GL_COLOR_BUFFER_BIT;
        }
        LY_ASSERT_MSG(false, "Invalid bit mask value!")
        return 0;
    }

    uint32_t renderer::get_renderer_cull_face_mode_to_opengl(renderer_cull_face_mode cull_face_mode)
    {
        LY_PROFILE_FUNCTION;
        switch (cull_face_mode)
        {
        case renderer_cull_face_mode::back:
            return GL_BACK;
        case renderer_cull_face_mode::front:
            return GL_FRONT;
        case renderer_cull_face_mode::both:
            return GL_FRONT_AND_BACK;
        }
        LY_ASSERT_MSG(false, "Invalid cull face mode value!")
        return 0;
    }

    void renderer::initialize_data()
    {
        LY_PROFILE_FUNCTION;
        s_data.clear_color = glm::vec4(0.85f);
        set_state(renderer_state::depth, true);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    }

    void renderer::open_gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                            const GLchar* message, const void* userParam)
    {
        LY_PROFILE_FUNCTION;
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            {
                LY_CRITICAL("OpenGL Debug Message (High Severity)");
                break;
            }
        case GL_DEBUG_SEVERITY_MEDIUM:
            {
                LY_WARN("OpenGL Debug Message (Medium Severity)");
                break;
            }
        case GL_DEBUG_SEVERITY_LOW:
            {
                LY_INFO("OpenGL Debug Message (Low Severity)");
                break;
            }
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            {
                LY_TRACE("OpenGL Debug Message (Notification)");
                break;
            }
        default:
            {
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
