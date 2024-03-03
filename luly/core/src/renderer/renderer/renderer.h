#pragma once

#include "renderer/window/window.h"
#include "renderer/arrays/vertex_array_object.h"
#include "renderer/models/model.h"

#include <memory>
#include <glm/glm.hpp>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "renderer/textures/texture.h"

namespace luly::renderer
{

#define CAMERA_UBO_LOCATION 0
#define LIGHTS_UBO_LOCATION 1
#define DIRECTIONAL_LIGHT_LIGHT_MATRICES_UBO_LOCATION 2
#define DIRECTIONAL_LIGHT_FRUSTUM_PLANES_UBO_LOCATION 3

    struct renderer_data
    {
        std::shared_ptr<window> window;
        glm::vec4 clear_color;
        bool vsync_enabled;
    };

    enum class renderer_draw_mode
    {
        points,
        line_strip,
        line_loop,
        lines,
        triangle_strip,
        triangle_fan,
        triangles
    };

    enum class renderer_state
    {
        depth,
        blend,
        face_cull
    };

    enum class renderer_depth_func
    {
        less,
        less_or_equal,
        equal,
        greater,
        greater_or_equal,
        not_equal,
        always,
        never
    };

    enum class renderer_cull_face_mode
    {
        front,
        back,
        both,
    };

    enum class renderer_bit_mask
    {
        depth = 0x00000100,
        stencil = 0x00000400,
        color = 0x00004000
    };

    inline renderer_bit_mask operator|(renderer_bit_mask lhs, renderer_bit_mask rhs)
    {
        return static_cast<renderer_bit_mask>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    class renderer
    {
    public:
        static void initialize(const std::shared_ptr<window>& window);

        static void swap_buffers();
        static void poll_input();
        static bool get_should_close();
        static void clear_screen();

        /* Getters */
        static glm::ivec2 get_viewport_size();
        static bool get_is_vsync_enabled();

        /* Setters*/
        static void set_vsync_enabled(bool vsync_enabled);
        static void set_viewport_size(const glm::ivec2& viewport_size);
        static void set_clear_color(const glm::vec4& clear_color);
        static void set_state(renderer_state state, bool is_enabled);
        static void set_depth_func(renderer_depth_func depth_func);
        static void set_cull_face_mode(renderer_cull_face_mode cull_face_mode);
        static void set_lines_thickness(float lines_thickness);

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

        /* Frame Buffers */
        static void blit_frame_buffer(const glm::ivec2& source_position, const glm::ivec2& source_dimensions,
                                      const glm::ivec2& target_position, const glm::ivec2& target_dimensions,
                                      renderer_bit_mask mask, texture_filtering filter);

        /* Utils */
        static uint32_t get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode);
        static uint32_t get_renderer_state_to_opengl(renderer_state state);
        static uint32_t get_renderer_depth_func_to_opengl(renderer_depth_func depth_func);
        static uint32_t get_renderer_bit_mask_to_opengl(renderer_bit_mask bit_mask);
        static uint32_t get_renderer_cull_face_mode_to_opengl(renderer_cull_face_mode cull_face_mode);

    private:
        static void initialize_data();
        static void initialize_glad();

        static void initialize_debug();

        static void open_gl_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                             const GLchar* message, const void* userParam);
        static renderer_data s_data;
    };
}
