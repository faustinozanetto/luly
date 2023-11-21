#pragma once

#include "window/window.h"
#include "arrays/vertex_array_object.h"
#include "models/model.h"
#include "renderer_api.h"

#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace luly::renderer
{
    struct LULY_RENDERER_API renderer_data
    {
        std::shared_ptr<window> window;
        glm::vec4 clear_color;
    };

    enum class LULY_RENDERER_API renderer_draw_mode
    {
        points,
        line_strip,
        line_loop,
        lines,
        triangle_strip,
        triangle_fan,
        triangles
    };

    class LULY_RENDERER_API renderer
    {
    public:
        static void initialize(const std::shared_ptr<window>& window);

        static void swap_buffers();
        static void poll_input();
        static bool get_should_close();
        static void clear_screen();
        static void set_clear_color(const glm::vec4& clear_color);

        /* Getters */
        static glm::ivec2 get_viewport_size();

        /* Setters*/
        static void set_vsync_enabled(bool vsync_enabled);

        /* Primitive Rendering */
        static void submit_arrays(int count, renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_arrays_instanced(int count, int instance_count,
                                            renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_elements(int count, renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* VAO Rendering */
        static void submit_vao(const std::shared_ptr<vertex_array_object>& vertex_array_object, int count,
                               renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_vao_instanced(const std::shared_ptr<vertex_array_object>& vao, int count, int instance_count,
                                         renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* Mesh Rendering */
        static void submit_mesh(const std::shared_ptr<mesh>& mesh,
                                renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* Model Rendering */
        static void submit_model(const std::shared_ptr<model>& model,
                                 renderer_draw_mode draw_mode = renderer_draw_mode::triangles);
        static void submit_model_instanced(const std::shared_ptr<model>& model, int instance_count,
                                           renderer_draw_mode draw_mode = renderer_draw_mode::triangles);

        /* Textures */
        static void bind_texture(int slot, uint32_t handle);

        /* Utils */
        static uint32_t get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode);

    private:
        static void initialize_data();
        static void initialize_glad();

        static void initialize_debug();

        static void open_gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                             const GLchar* message, const void* userParam);
        static renderer_data s_data;
    };
}
