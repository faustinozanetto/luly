﻿#include "lypch.h"
#include "environment_pass.h"

#include "assets/asset_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/render_buffer/render_buffer.h"
#include "renderer/shaders/shader_factory.h"
#include "renderer/textures/texture_factory.h"
#include "scene/actor/components/rendering/skybox_component.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

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
        LY_PROFILE_FUNCTION;
        // Setup environment
        setup_environment();
    }

    void environment_pass::execute()
    {
        LY_PROFILE_FUNCTION;
    }

    void environment_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output irradiance_output;
        irradiance_output.name = "irradiance_output";
        irradiance_output.output = m_environment_irradiance_texture->get_handle_id();
        add_output(irradiance_output);

        render_pass_output prefilter_output;
        prefilter_output.name = "prefilter_output";
        prefilter_output.output = m_environment_prefilter_texture->get_handle_id();
        add_output(prefilter_output);

        render_pass_output brdf_output;
        brdf_output.name = "brdf_output";
        brdf_output.output = m_brdf_texture->get_handle_id();
        add_output(brdf_output);

        render_pass_output environment_cubemap_texture;
        environment_cubemap_texture.name = "environment_cubemap_output";
        environment_cubemap_texture.output = m_environment_cubemap_texture->get_handle_id();
        add_output(environment_cubemap_texture);
    }

    void environment_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
    }

    void environment_pass::set_environment_map(const std::shared_ptr<texture_2d>& environment_map)
    {
        LY_PROFILE_FUNCTION;
        m_environment_cubemap_texture.reset();
        m_brdf_texture.reset();
        m_environment_prefilter_texture.reset();
        m_environment_irradiance_texture.reset();
        m_environment_hdr_texture.reset();

        m_environment_hdr_texture = environment_map;

        setup_environment_fbo();
        setup_environment_cubemap();
        setup_environment_equirectangular_map();
        setup_environment_equirectangular_map();
        setup_irradiance_map();
        setup_prefilter_map();
        setup_brdf_map();

        set_outputs();

        renderer::set_viewport_size(renderer::get_viewport_size());
    }

    void environment_pass::setup_environment()
    {
        LY_PROFILE_FUNCTION;
        m_irradiance_map_size = 128;
        m_prefilter_map_size = 512;
        m_brdf_map_size = 512;
        m_environment_map_size = 2048;

        m_cube_mesh = mesh_factory::create_cube_mesh();
        m_quad_mesh = mesh_factory::create_screen_quad_mesh();

        m_equirectangular_to_cubemap_shader = shader_factory::create_shader_from_file(
            "assets/shaders/skybox/equirectangular_to_cubemap.lsh");
        assets::asset_factory::create_asset("equirectangular_to_cubemap-shader", assets::asset_type::shader,
                                            m_equirectangular_to_cubemap_shader);

        m_irradiance_shader = shader_factory::create_shader_from_file("assets/shaders/skybox/irradiance.lsh");
        assets::asset_factory::create_asset("irradiance-shader", assets::asset_type::shader, m_irradiance_shader);

        m_prefilter_shader = shader_factory::create_shader_from_file("assets/shaders/skybox/prefilter.lsh");
        assets::asset_factory::create_asset("prefilter-shader", assets::asset_type::shader, m_prefilter_shader);

        m_brdf_shader = shader_factory::create_shader_from_file("assets/shaders/skybox/brdf.lsh");
        assets::asset_factory::create_asset("brdf-shader", assets::asset_type::shader, m_brdf_shader);
    }

    void environment_pass::setup_environment_fbo()
    {
        LY_PROFILE_FUNCTION;
        glm::ivec2 viewport_size = renderer::get_viewport_size();
        m_environment_capture_fbo = std::make_shared<frame_buffer>(viewport_size.x, viewport_size.y);
        m_environment_capture_rbo = std::make_shared<render_buffer>(
            m_environment_map_size, m_environment_map_size,
            texture_internal_format::depth_component24);
        m_environment_capture_rbo->attach_to_frame_buffer(render_buffer_attachment_type::depth);
        m_environment_capture_fbo->initialize();
    }

    void environment_pass::setup_environment_cubemap()
    {
        LY_PROFILE_FUNCTION;
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
        LY_PROFILE_FUNCTION;
        // Skybox texture.
        m_capture_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        m_capture_views =
        {
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
            lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
        };

        m_equirectangular_to_cubemap_shader->bind();
        m_equirectangular_to_cubemap_shader->set_mat4("u_projection_matrix", m_capture_projection);
        renderer::bind_texture(0, m_environment_hdr_texture->get_handle_id());

        renderer::set_viewport_size({m_environment_map_size, m_environment_map_size});
        m_environment_capture_fbo->bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_equirectangular_to_cubemap_shader->set_mat4("u_view_matrix", m_capture_views[i]);
            m_environment_capture_fbo->attach_texture(m_environment_cubemap_texture, GL_FRAMEBUFFER,
                                                      render_buffer_attachment_type::color,
                                                      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, false);
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
        LY_PROFILE_FUNCTION;
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
        //   renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_environment_cubemap_texture->get_handle_id());

        renderer::set_viewport_size({m_irradiance_map_size, m_irradiance_map_size});
        m_environment_capture_fbo->bind();

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_irradiance_shader->set_mat4("u_view_matrix", m_capture_views[i]);
            m_environment_capture_fbo->attach_texture(m_environment_irradiance_texture, GL_FRAMEBUFFER,
                                                      render_buffer_attachment_type::color,
                                                      GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, false);

            renderer::clear_screen();
            renderer::submit_mesh(m_cube_mesh);
        }
        m_environment_capture_fbo->un_bind();
        m_environment_capture_rbo->un_bind();
        m_irradiance_shader->un_bind();
    }

    void environment_pass::setup_prefilter_map()
    {
        LY_PROFILE_FUNCTION;
        texture_specification texture_specification;
        texture_specification.width = m_prefilter_map_size;
        texture_specification.height = m_prefilter_map_size;
        texture_specification.channels = 3;
        texture_specification.internal_format = texture_internal_format::rgb16f;
        texture_specification.data = nullptr;

        m_environment_prefilter_texture = std::make_shared<texture_cubemap>(texture_specification);
        // Generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
        // ----------------------------------------------------------------------------------------------------
        m_prefilter_shader->bind();
        m_prefilter_shader->set_mat4("u_projection_matrix", m_capture_projection);
        //  renderer::bind_texture(0, m_environment_cubemap_texture->get_handle_id());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_environment_cubemap_texture->get_handle_id());

        m_environment_capture_fbo->bind();
        const uint32_t max_mip_map_levels = glm::log2(static_cast<float>(m_prefilter_map_size));
        for (uint32_t mip = 0; mip < max_mip_map_levels; ++mip)
        {
            // Resize framebuffer according to mip-level size.
            const uint32_t mip_width = static_cast<uint32_t>(m_prefilter_map_size * std::pow(0.5, mip));
            const uint32_t mip_height = static_cast<uint32_t>(m_prefilter_map_size * std::pow(0.5, mip));

            //m_environment_capture_rbo->bind();
            m_environment_capture_rbo->set_storage_parameters(mip_width, mip_height,
                                                              texture_internal_format::depth_component24);
            renderer::set_viewport_size({mip_width, mip_height});

            const float roughness = static_cast<float>(mip) / static_cast<float>(max_mip_map_levels - 1);
            m_prefilter_shader->set_float("u_roughness", roughness);

            for (uint32_t i = 0; i < 6; ++i)
            {
                m_prefilter_shader->set_mat4("u_view_matrix", m_capture_views[i]);
                /*
                                m_environment_capture_fbo->attach_texture(m_environment_prefilter_texture, GL_FRAMEBUFFER,
                                                                          render_buffer_attachment_type::color,
                                                                          GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, mip, false);
                                                                          */
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                                       m_environment_prefilter_texture->get_handle_id(), mip);

                renderer::clear_screen();
                renderer::submit_mesh(m_cube_mesh);
            }
        }
        m_environment_capture_fbo->un_bind();
    }

    void environment_pass::setup_brdf_map()
    {
        LY_PROFILE_FUNCTION;
        texture_specification texture_specification;
        texture_specification.width = m_brdf_map_size;
        texture_specification.height = m_brdf_map_size;
        texture_specification.channels = 2;
        texture_specification.internal_format = texture_internal_format::rg16f;
        texture_specification.data = nullptr;

        m_brdf_texture = std::make_shared<texture_2d>(texture_specification);

        m_environment_capture_fbo->bind();
        m_environment_capture_rbo->bind();
        m_environment_capture_rbo->set_storage_parameters(m_brdf_map_size, m_brdf_map_size,
                                                          texture_internal_format::depth_component24);
        m_environment_capture_fbo->attach_texture(m_brdf_texture, GL_FRAMEBUFFER, render_buffer_attachment_type::color,
                                                  GL_TEXTURE_2D, false);

        renderer::set_viewport_size({m_brdf_map_size, m_brdf_map_size});
        m_brdf_shader->bind();
        renderer::clear_screen();

        renderer::submit_mesh(m_quad_mesh, renderer_draw_mode::triangle_strip);
        m_environment_capture_fbo->un_bind();
    }
}
