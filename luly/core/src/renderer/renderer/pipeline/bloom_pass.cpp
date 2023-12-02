﻿#include "lypch.h"
#include "bloom_pass.h"

#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    bloom_pass::bloom_pass() : render_pass("bloom_pass")
    {
        bloom_pass::initialize();
    }

    bloom_pass::~bloom_pass()
    {
    }

    void bloom_pass::initialize()
    {
        // Setup pass frame buffer.
        glm::ivec2 viewport_size_int = renderer::get_viewport_size();
        glm::vec2 viewport_size = glm::vec2(viewport_size_int);

        m_samples_count = 6;
        m_filter_radius = 0.005f;
        m_strength = 0.01f;
        m_mips.resize(m_samples_count);

        m_fbo = std::make_shared<frame_buffer>(viewport_size.x, viewport_size.y);

        for (int i = 0; i < m_samples_count; i++)
        {
            bloom_pass_mip_data bloom_pass_mip_data;
            viewport_size *= 0.5f;
            viewport_size_int /= 2;

            bloom_pass_mip_data.size = viewport_size_int;

            texture_specification mip_texture_specification;
            mip_texture_specification.width = bloom_pass_mip_data.size.x;
            mip_texture_specification.height = bloom_pass_mip_data.size.y;
            mip_texture_specification.channels = 3;
            mip_texture_specification.mip_map_levels = 1;
            mip_texture_specification.internal_format = texture_internal_format::r11g11b10;
            mip_texture_specification.data = nullptr;

            bloom_pass_mip_data.texture = std::make_shared<texture_2d>(mip_texture_specification);

            m_mips[i] = bloom_pass_mip_data;
        }

        // Attach first bloom mip texture to rbo.
        m_fbo->attach_texture(m_mips[0].texture, GL_FRAMEBUFFER,
                              render_buffer_attachment_type::color, GL_TEXTURE_2D,
                              true, 0);
        m_fbo->initialize();

        // Load shaders
        m_down_sample_shader = shader_factory::create_shader_from_file(
            "assets/shaders/bloom/bloom_down_sample.lsh");
        m_up_sample_shader = shader_factory::create_shader_from_file(
            "assets/shaders/bloom/bloom_up_sample.lsh");
        m_composition_shader = shader_factory::create_shader_from_file(
            "assets/shaders/bloom/bloom_composition.lsh");
        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void bloom_pass::execute()
    {
        m_fbo->bind();

        perform_down_sample();
        perform_up_sample();
        perform_composition();

        m_fbo->un_bind();

        renderer::set_viewport_size(renderer::get_viewport_size());
    }

    void bloom_pass::set_outputs()
    {
        render_pass_output bloom_output;
        bloom_output.name = "bloom_output";
        bloom_output.output = m_fbo->get_attachment_id(0);
        add_output(bloom_output);
    }

    void bloom_pass::on_resize(const glm::ivec2& dimensions)
    {
        //  m_fbo->resize(dimensions);
    }

    void bloom_pass::perform_down_sample()
    {
        m_down_sample_shader->bind();

        const render_pass_input& skybox_pass_input = m_inputs.at("skybox_pass_input");
        const render_pass_output& skybox_pass_output = skybox_pass_input.render_pass->get_output("skybox_output");

        // Bind skybox pass output.
        renderer::bind_texture(0, skybox_pass_output.output);

        // Submit uniforms
        m_down_sample_shader->set_vec_float2("u_source_res", renderer::get_viewport_size());
        m_down_sample_shader->set_int("u_mip_level", 0);

        for (int i = 0; i < m_mips.size(); i++)
        {
            bloom_pass_mip_data& current_bloom_mip = m_mips[i];
            renderer::set_viewport_size(current_bloom_mip.size);
            // Attach bloom mip textue to rbo.
            m_fbo->attach_texture(current_bloom_mip.texture, GL_FRAMEBUFFER, render_buffer_attachment_type::color,
                                  GL_TEXTURE_2D, false, 0);

            // Draw screen mesh
            renderer::submit_mesh(m_screen_mesh);

            // Set current mip resolution as srcResolution for next iteration
            m_down_sample_shader->set_vec_float2("u_source_res", current_bloom_mip.size);
            // Set current mip as texture input for next iteration
            renderer::bind_texture(0, current_bloom_mip.texture->get_handle_id());
            // Disable Karis average for consequent down samples
            if (i == 0)
            {
                m_down_sample_shader->set_int("u_mip_level", 1);
            }
        }
        m_down_sample_shader->un_bind();
    }

    void bloom_pass::perform_up_sample()
    {
        // Set state for up sample pass.
        renderer::set_state(renderer_state::blend, true);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendEquation(GL_FUNC_ADD);

        m_up_sample_shader->bind();
        m_up_sample_shader->set_float("u_filter_radius", m_filter_radius);
        for (int i = m_mips.size() - 1; i > 0; i--)
        {
            bloom_pass_mip_data& current_bloom_mip = m_mips[i];
            bloom_pass_mip_data& next_bloom_mip = m_mips[i - 1];
            // Bind mip texture
            renderer::bind_texture(0, current_bloom_mip.texture->get_handle_id());
            renderer::set_viewport_size(next_bloom_mip.size);
            // Attach bloom mip texture to render buffer object.
            m_fbo->attach_texture(next_bloom_mip.texture, GL_FRAMEBUFFER,
                                  render_buffer_attachment_type::color, GL_TEXTURE_2D, false, 0);

            // Draw screen mesh
            renderer::submit_mesh(m_screen_mesh);
        }

        m_up_sample_shader->un_bind();

        // Reset state.
        renderer::set_state(renderer_state::blend, false);
    }

    void bloom_pass::perform_composition()
    {
        m_composition_shader->bind();

        const render_pass_input& skybox_pass_input = m_inputs.at("skybox_pass_input");
        const render_pass_output& skybox_pass_output = skybox_pass_input.render_pass->get_output("skybox_output");

        m_composition_shader->set_float("u_strength", m_strength);

        // Bind skybox pass output.
        renderer::bind_texture(0, skybox_pass_output.output);
        renderer::bind_texture(1, m_mips[0].texture->get_handle_id()); // Bloom output

        // Draw screen mesh
        renderer::submit_mesh(m_screen_mesh);

        m_composition_shader->un_bind();
    }
}