#include "lypch.h"
#include "final_pass.h"

#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    final_pass::final_pass() : render_pass("final_pass")
    {
        final_pass::initialize();
    }

    final_pass::~final_pass()
    {
    }

    void final_pass::initialize()
    {
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

        // Create shader.
        m_screen_shader = shader_factory::create_shader_from_file("assets/shaders/screen.lsh");

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void final_pass::execute()
    {
        m_fbo->bind();
        renderer::clear_screen();
        m_screen_shader->bind();

        const render_pass_input& skybox_pass_input = m_inputs.at("skybox_pass_input");
        const render_pass_output& skybox_output = skybox_pass_input.render_pass->get_output("skybox_output");

        renderer::bind_texture(0, skybox_output.output);
        renderer::submit_mesh(m_screen_mesh);

        m_screen_shader->un_bind();
        m_fbo->un_bind();
    }

    void final_pass::set_outputs()
    {
        render_pass_output final_output;
        final_output.name = "final_output";
        final_output.output = m_fbo->get_attachment_id(0);
        add_output(final_output);
    }

    void final_pass::on_resize(const glm::ivec2& dimensions)
    {
        m_fbo->resize(dimensions);
    }
}
