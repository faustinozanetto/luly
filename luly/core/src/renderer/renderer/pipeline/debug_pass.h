#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    struct debug_line_vertex
    {
        glm::vec3 position;
        glm::vec3 color;
    };

    struct debug_pass_data
    {
        uint32_t max_quads = 20000;
        uint32_t max_vertices = max_quads * 4;

        std::shared_ptr<vertex_array_object> line_vao;
        std::shared_ptr<vertex_buffer_object> line_vbo;

        uint32_t line_vertex_count = 0;
        debug_line_vertex* line_vertex_buffer_base = nullptr;
        debug_line_vertex* line_vertex_buffer_ptr = nullptr;
    };

    class debug_pass : public render_pass
    {
    public:
        debug_pass();
        ~debug_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

        /* Methods */
        void reset_stats();

    private:
        std::shared_ptr<shader> m_debug_lines_shader;
        std::shared_ptr<mesh> m_screen_mesh;
        debug_pass_data s_data;
    };
}
