#include "lypch.h"
#include "debug_pass.h"

#include "assets/asset_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/debug/debug_renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/scene.h"

namespace luly::renderer
{
    debug_pass::debug_pass() : render_pass("debug_pass")
    {
        debug_pass::initialize();
    }

    debug_pass::~debug_pass()
    {
    }

    void debug_pass::initialize()
    {
        LY_PROFILE_FUNCTION;
        // Setup pass frame buffer.
        const glm::ivec2& viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge, viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);
        m_fbo->initialize();

        // Setup debug lines
        s_data.line_vertex_count = 0;
        s_data.line_vertex_buffer_ptr = nullptr;
        s_data.line_vertex_buffer_base = nullptr;

        // Load shaders
        m_debug_lines_shader = shader_factory::create_shader_from_file("assets/shaders/debug/debug_line.lsh");
        assets::asset_factory::create_asset("debug_line-shader", assets::asset_type::shader, m_debug_lines_shader);

        /* Create debug lines VAO and VBO */
        s_data.line_vao = std::make_shared<vertex_array_object>();
        s_data.line_vao->bind();
        s_data.line_vbo = std::make_shared<vertex_buffer_object>();
        s_data.line_vbo->set_data(vertex_buffer_object_usage::dynamic_draw,
                                  s_data.max_vertices * sizeof(debug_line_vertex),
                                  nullptr);

        std::initializer_list<vertex_buffer_layout_entry>
            layout_elements = {
                {"a_pos", vertex_buffer_entry_type::FLOAT3, false},
                {"a_color", vertex_buffer_entry_type::FLOAT3, false}
            };

        const auto vbo_layout_descriptor = std::make_shared<
            vertex_buffer_layout_descriptor>(layout_elements);
        s_data.line_vbo->set_layout_descriptor(vbo_layout_descriptor);
        s_data.line_vao->add_vertex_buffer(s_data.line_vbo);

        /* Allocate debug line vertices */
        s_data.line_vertex_buffer_base = new debug_line_vertex[s_data.max_vertices];
    }

    void debug_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        renderer::set_lines_thickness(debug_renderer::get_data().lines_thickness);
        m_fbo->bind();
        renderer::clear_screen();
        m_debug_lines_shader->bind();

        const render_pass_input& debanding_pass_input = m_inputs.at("debanding_pass_input");

        // Blit color buffer from previous pass to this fbo.
        int width = m_fbo->get_width();
        int height = m_fbo->get_height();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, debanding_pass_input.render_pass->get_fbo()->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->get_handle_id());
        renderer::blit_frame_buffer({0, 0}, {width, height}, {0, 0}, {width, height}, renderer_bit_mask::color,
                                    texture_filtering::nearest);

        const std::vector<debug_line>& debug_lines = debug_renderer::get_data().debug_lines;
        for (const debug_line& line : debug_lines)
        {
            s_data.line_vertex_buffer_ptr->position = line.point_a;
            s_data.line_vertex_buffer_ptr->color = line.color;
            s_data.line_vertex_buffer_ptr++;

            s_data.line_vertex_buffer_ptr->position = line.point_b;
            s_data.line_vertex_buffer_ptr->color = line.color;
            s_data.line_vertex_buffer_ptr++;

            s_data.line_vertex_count += 2;
        }

        const uint32_t size = static_cast<uint32_t>(reinterpret_cast<uint8_t*>(s_data.line_vertex_buffer_ptr) -
            reinterpret_cast<uint8_t*>(s_data.
                line_vertex_buffer_base));
        s_data.line_vbo->set_data(vertex_buffer_object_usage::dynamic_draw, size,
                                  s_data.line_vertex_buffer_base);


        renderer::submit_vao(s_data.line_vao, s_data.line_vertex_count, renderer_draw_mode::lines);

        m_debug_lines_shader->un_bind();
        m_fbo->un_bind();

        debug_renderer::reset();
        renderer::set_lines_thickness(1.0f);
    }

    void debug_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output debug_output;
        debug_output.name = "debug_output";
        debug_output.output = m_fbo->get_attachment_id(0);
        add_output(debug_output);
    }

    void debug_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }

    void debug_pass::reset_stats()
    {
        s_data.line_vertex_count = 0;
        s_data.line_vertex_buffer_ptr = s_data.line_vertex_buffer_base;
    }
}
