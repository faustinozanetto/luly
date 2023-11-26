#include "lypch.h"
#include "skybox_pass.h"

#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    skybox_pass::skybox_pass(): render_pass("skybox_pass")
    {
        skybox_pass::initialize();
    }

    skybox_pass::~skybox_pass()
    {
    }

    void skybox_pass::initialize()
    {
        auto viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32f,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge, viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);
        m_fbo->initialize();

        m_skybox_shader = shader_factory::create_shader_from_file("assets/shaders/skybox/skybox.lsh");
        m_screen_shader = shader_factory::create_shader_from_file("assets/shaders/screen.lsh");
        m_cube_mesh = mesh_factory::create_cube_mesh();
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void skybox_pass::execute()
    {
        const render_pass_input& geometry_input = m_inputs.at("geometry_pass_input");
        const std::shared_ptr<frame_buffer>& geometry_input_fbo = geometry_input.render_pass->get_frame_buffer();

        const render_pass_input& lighting_input = m_inputs.at("lighting_pass_input");
        const std::shared_ptr<frame_buffer>& lighting_input_fbo = lighting_input.render_pass->get_frame_buffer();

        const render_pass_input& environment_pass_input = m_inputs.at("environment_pass_input");
        const render_pass_output& environment_cubemap_texture = environment_pass_input.render_pass->get_output(
            "environment_cubemap_output");

        lighting_input_fbo->bind();

        renderer::set_state(renderer_state::depth, true);
        renderer::set_depth_func(renderer_depth_func::less_or_equal);

        int width = m_fbo->get_width();
        int height = m_fbo->get_height();
        
        // Copy color buffer from lighting pass to this fbo.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, lighting_input_fbo->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->get_handle_id());
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // Copy depth buffer from geometry pass to this fbo.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, geometry_input_fbo->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->get_handle_id());
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

/*
        glBindFramebuffer(GL_READ_FRAMEBUFFER, geometry_input_fbo->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lighting_input_fbo->get_handle_id());
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        */
        // Render skybox cube using the environment cubemap texture.
        m_skybox_shader->bind();
        renderer::bind_texture(0, environment_cubemap_texture.pass_output->get_handle_id());
        renderer::submit_mesh(m_cube_mesh);
        m_skybox_shader->un_bind();

        renderer::set_depth_func(renderer_depth_func::less);

        lighting_input_fbo->un_bind();
    }

    void skybox_pass::set_outputs()
    {
    }
}
