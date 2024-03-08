#include "lypch.h"
#include "antialiasing_pass.h"

#include "assets/assets_manager.h"
#include "assets/asset_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    antialiasing_pass::antialiasing_pass() : render_pass("antialiasing_pass")
    {
        antialiasing_pass::initialize();
    }

    antialiasing_pass::~antialiasing_pass()
    {
    }

    void antialiasing_pass::initialize()
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
        if (!assets::assets_manager::get().asset_already_registered("fxaa-shader"))
        {
            m_antialiasing_shader = shader_factory::create_shader_from_file("assets/shaders/fxaa.lsh");
            assets::asset_factory::create_asset("fxaa-shader", assets::asset_type::shader, m_antialiasing_shader);
        }
        else
        {
            m_antialiasing_shader = assets::assets_manager::get().get_asset<assets::asset, assets::asset_type::shader>(
                "fxaa-shader")->get_data<shader>();
        }

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void antialiasing_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        m_fbo->bind();
        renderer::clear_screen();
        m_antialiasing_shader->bind();

        const render_pass_input& tonemapping_pass_input = m_inputs.at("tonemapping_pass_input");
        const render_pass_output& tonemapping_pass_output = tonemapping_pass_input.render_pass->get_output(
            "tonemapping_output");

        m_antialiasing_shader->set_vec_float2("u_screen_size", renderer::get_viewport_size());
        renderer::bind_texture(0, tonemapping_pass_output.output);
        renderer::submit_mesh(m_screen_mesh);

        m_antialiasing_shader->un_bind();
        m_fbo->un_bind();
    }

    void antialiasing_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output antialiasing_output;
        antialiasing_output.name = "antialiasing_output";
        antialiasing_output.output = m_fbo->get_attachment_id(0);
        add_output(antialiasing_output);
    }

    void antialiasing_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }
}
