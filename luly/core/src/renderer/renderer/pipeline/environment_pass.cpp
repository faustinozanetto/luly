#include "lypch.h"
#include "environment_pass.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/render_buffer/render_buffer.h"
#include "renderer/shaders/shader_factory.h"
#include "renderer/textures/texture_factory.h"

namespace luly::renderer
{
    environment_pass::environment_pass() : render_pass("environment_pass")
    {
        environment_pass::initialize();
    }

    environment_pass::~environment_pass()
    {
    }

    void environment_pass::initialize()
    {
        auto viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments);

        setup_environment();

        render_pass_output irradiance_output;
        irradiance_output.name = "irradiance_output";
        irradiance_output.pass_output = m_environment_irradiance_texture;
        add_output(irradiance_output);
    }

    void environment_pass::execute()
    {
    }

    void environment_pass::setup_environment()
    {
        m_irradiance_map_size = 128;
        m_prefilter_map_size = 1024;
        m_brdf_map_size = 1024;
        m_environment_map_size = 2048;

        m_cube_mesh = mesh_factory::create_cube_mesh();

        m_equirectangular_to_cubemap_shader = shader_factory::create_shader_from_file(
            "assets/shaders/skybox/equirectangular_to_cubemap.lsh");
        m_irradiance_shader = shader_factory::create_shader_from_file("assets/shaders/skybox/irradiance.lsh");

        setup_environment_fbo();
        setup_environment_texture();
        setup_environment_cubemap();
        setup_environment_equirectangular_map();
        setup_irradiance_map();

        renderer::set_viewport_size(renderer::get_viewport_size());
    }

    void environment_pass::setup_environment_fbo()
    {
        glm::ivec2 viewport_size = renderer::get_viewport_size();
        m_environment_capture_fbo = std::make_shared<frame_buffer>(viewport_size.x, viewport_size.y);
        m_environment_capture_rbo = std::make_shared<render_buffer>(
            m_environment_map_size, m_environment_map_size,
            texture_internal_format::depth_component24);
        m_environment_capture_rbo->attach_to_frame_buffer(render_buffer_attachment_type::depth);
    }

    void environment_pass::setup_environment_texture()
    {
        m_environment_hdr_texture = texture_factory::create_environment_texture_from_file(
            "assets/hdris/meadow_2_4k.hdr");
    }

    void environment_pass::setup_environment_cubemap()
    {
        texture_specification texture_specification;
        texture_specification.width = m_environment_map_size;
        texture_specification.height = m_environment_map_size;
        texture_specification.channels = 3;
        texture_specification.internal_format = texture_internal_format::rgb16f;
        texture_specification.data = nullptr;

        m_environment_cubemap_texture = std::make_shared<texture_cubemap>(texture_specification);
    }

    void environment_pass::setup_environment_equirectangular_map()
    {
        // Skyobox texture.
        m_capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        m_capture_views =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };

        m_equirectangular_to_cubemap_shader->bind();
        m_equirectangular_to_cubemap_shader->set_mat4("u_projection_matrix", m_capture_projection);
        renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());

        renderer::set_viewport_size({m_environment_map_size, m_environment_map_size});
        m_environment_capture_fbo->bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_equirectangular_to_cubemap_shader->set_mat4("u_view_matrix", m_capture_views[i]);
            m_environment_capture_fbo->attach_texture(m_environment_cubemap_texture, GL_FRAMEBUFFER,
                                                      render_buffer_attachment_type::color,
                                                      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0);
            renderer::clear_screen();

            renderer::submit_mesh(m_cube_mesh);
        }

        m_environment_capture_fbo->un_bind();
        m_equirectangular_to_cubemap_shader->un_bind();

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_environment_cubemap_texture->get_handle_id());
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    }

    void environment_pass::setup_irradiance_map()
    {
        texture_specification texture_specification;
        texture_specification.width = m_irradiance_map_size;
        texture_specification.height = m_irradiance_map_size;
        texture_specification.channels = 3;
        texture_specification.internal_format = texture_internal_format::rgb16f;
        texture_specification.data = nullptr;

        m_environment_irradiance_texture = std::make_shared<texture_cubemap>(texture_specification);

        m_environment_capture_fbo->bind();
        m_environment_capture_rbo->bind();
        m_environment_capture_rbo->set_storage_parameters(m_irradiance_map_size, m_irradiance_map_size,
                                                          texture_internal_format::depth_component24);

        m_irradiance_shader->bind();
        m_irradiance_shader->set_mat4("u_projection_matrix", m_capture_projection);
        renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());

        renderer::set_viewport_size({m_irradiance_map_size, m_irradiance_map_size});
        m_environment_capture_fbo->bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_irradiance_shader->set_mat4("u_view_matrix", m_capture_views[i]);
            m_environment_capture_fbo->attach_texture(m_environment_irradiance_texture, GL_FRAMEBUFFER,
                                                      render_buffer_attachment_type::color,
                                                      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0);

            renderer::clear_screen();
            renderer::submit_mesh(m_cube_mesh);
        }
        m_environment_capture_fbo->un_bind();
        m_environment_capture_rbo->un_bind();
        m_irradiance_shader->un_bind();
    }
}
