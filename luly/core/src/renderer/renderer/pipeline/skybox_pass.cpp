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

        const render_pass_input& lighting_pass_input = m_inputs.at("lighting_pass_input");
        const std::shared_ptr<frame_buffer>& lighting_input_fbo = lighting_pass_input.render_pass->get_frame_buffer();

        const render_pass_input& environment_pass_input = m_inputs.at("environment_pass_input");
        const render_pass_output& environment_cubemap_texture = environment_pass_input.render_pass->get_output(
            "environment_cubemap_output");

        lighting_input_fbo->bind();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, geometry_input_fbo->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, lighting_input_fbo->get_handle_id());
        int width = m_fbo->get_width();
        int height = m_fbo->get_height();
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

        renderer::set_state(renderer_state::depth, true);
        glDepthFunc(GL_LEQUAL);
        m_skybox_shader->bind();
        renderer::bind_texture(0, environment_cubemap_texture.pass_output->get_handle_id());
        renderer::submit_mesh(m_cube_mesh);
        m_skybox_shader->un_bind();

        glDepthFunc(GL_LESS);

        lighting_input_fbo->un_bind();

        m_fbo->bind();
        m_screen_shader->bind();
        renderer::bind_texture(0, lighting_input_fbo->get_attachment_id(0));
        renderer::submit_mesh(m_screen_mesh);
        m_screen_shader->un_bind();
        m_fbo->un_bind();
    }
}
