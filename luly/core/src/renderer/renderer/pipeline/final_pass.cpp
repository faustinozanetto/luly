﻿#include "lypch.h"
#include "final_pass.h"

#include "assets/assets_manager.h"
#include "assets/asset_factory.h"
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
        if (!assets::assets_manager::get().asset_already_registered("screen-shader"))
        {
            m_screen_shader = shader_factory::create_shader_from_file("assets/shaders/screen.lsh");
            assets::asset_factory::create_asset("screen-shader", assets::asset_type::shader, m_screen_shader);
        }
        else
        {
            m_screen_shader = assets::assets_manager::get().get_asset<assets::asset, assets::asset_type::shader>(
                "screen-shader")->get_data<shader>();
        }

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void final_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        m_fbo->bind();
        renderer::clear_screen();
        m_screen_shader->bind();

        const render_pass_input& debug_pass_input = m_inputs.at("debug_pass_input");
        const render_pass_output& debug_pass_output = debug_pass_input.render_pass->get_output(
            "debug_output");

        renderer::bind_texture(0, debug_pass_output.output);
        renderer::submit_mesh(m_screen_mesh);

        m_screen_shader->un_bind();
        m_fbo->un_bind();
    }

    void final_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output final_output;
        final_output.name = "final_output";
        final_output.output = m_fbo->get_attachment_id(0);
        add_output(final_output);
    }

    void final_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }
}
