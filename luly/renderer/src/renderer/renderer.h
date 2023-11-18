#pragma once

#include "window/window.h"

#include <memory>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "arrays/vertex_array_object.h"

namespace luly::renderer
{
    struct renderer_data
    {
        std::shared_ptr<window> window;
        glm::vec4 clear_color;
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

    class renderer
    {
    public:
        static void initialize();

        static void swap_buffers();
        static void poll_input();
        static bool get_should_close();
        static void clear_screen();
        static void set_clear_color(const glm::vec4& clear_color);

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

        /* Utils */
        static uint32_t get_renderer_draw_mode_to_opengl(renderer_draw_mode draw_mode);

    private:
        static void initialize_data();
        static void initialize_glad();
    };
}
