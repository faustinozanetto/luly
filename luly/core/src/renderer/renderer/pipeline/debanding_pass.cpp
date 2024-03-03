#include "lypch.h"
#include "debanding_pass.h"

#include "assets/asset_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    debanding_pass::debanding_pass() : render_pass("debanding_pass")
    {
        debanding_pass::initialize();
    }

    debanding_pass::~debanding_pass()
    {
    }

    void debanding_pass::initialize()
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

        // Create shader.
        m_debanding_shader = shader_factory::create_shader_from_file("assets/shaders/debanding.lsh");
        assets::asset_factory::create_asset("debanding-shader", assets::asset_type::shader, m_debanding_shader);

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
        m_intensity = 0.02f;
        m_threshold = 0.005f;
    }

    void debanding_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        m_fbo->bind();
        renderer::clear_screen();
        m_debanding_shader->bind();

        const render_pass_input& tonemapping_pass_input = m_inputs.at("tonemapping_pass_input");
        const render_pass_output& tonemapping_output = tonemapping_pass_input.render_pass->get_output(
            "tonemapping_output");

        m_debanding_shader->set_float("u_intensity", m_intensity);
        m_debanding_shader->set_float("u_threshold", m_threshold);
        renderer::bind_texture(0, tonemapping_output.output);
        renderer::submit_mesh(m_screen_mesh);

        m_debanding_shader->un_bind();
        m_fbo->un_bind();
    }

    void debanding_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output debanding_output;
        debanding_output.name = "debanding_output";
        debanding_output.output = m_fbo->get_attachment_id(0);
        add_output(debanding_output);
    }

    void debanding_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }
}
