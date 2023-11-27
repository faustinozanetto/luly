﻿#include "lypch.h"
#include "lighting_pass.h"

#include "application/application.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/lights/spot_light_component.h"

namespace luly::renderer
{
    lighting_pass::lighting_pass() : render_pass("lighting_pass")
    {
        lighting_pass::initialize();
    }

    lighting_pass::~lighting_pass()
    {
    }

    void lighting_pass::initialize()
    {
        // Create pass frame buffer.
        const glm::ivec2& viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::rgba16,
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

        // Setup lights uniform buffer object.
        initialize_lights_data();
        m_lights_ubo = std::make_shared<uniform_buffer_object>(
            sizeof(m_point_lights) + sizeof(m_spot_lights) + sizeof(m_directional_light), 1);

        // Load shader.
        m_lighting_shader = shader_factory::create_shader_from_file("assets/shaders/lighting_pass_shader.lsh");

        m_screen_mesh = mesh_factory::create_screen_quad_mesh();

        set_outputs();
    }

    void lighting_pass::execute()
    {
        renderer::set_state(renderer_state::depth, false);
        m_fbo->bind();
        renderer::clear_screen();
        m_lighting_shader->bind();

        const render_pass_input& geometry_pass_input = m_inputs.at("geometry_pass_input");
        const render_pass_input& environment_pass_input = m_inputs.at("environment_pass_input");
        const render_pass_input& ambient_occlusion_pass_input = m_inputs.at("ambient_occlusion_pass_input");

        const render_pass_output& geometry_position_output = geometry_pass_input.render_pass->get_output(
            "position_output");
        const render_pass_output& geometry_albedo_output = geometry_pass_input.render_pass->get_output("albedo_output");
        const render_pass_output& geometry_normals_output = geometry_pass_input.render_pass->get_output(
            "normals_output");
        const render_pass_output& geometry_rough_metal_ao_output = geometry_pass_input.render_pass->get_output(
            "rough_metal_ao_output");

        int width = m_fbo->get_width();
        int height = m_fbo->get_height();

        glBindFramebuffer(GL_READ_FRAMEBUFFER, geometry_pass_input.render_pass->get_fbo()->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->get_handle_id());
        renderer::blit_frame_buffer({0, 0}, {width, height}, {0, 0}, {width, height}, renderer_bit_mask::depth,
                                    texture_filtering::nearest);

        // Bind geometry pass outputs.
        renderer::bind_texture(0, geometry_position_output.output);
        renderer::bind_texture(1, geometry_albedo_output.output);
        renderer::bind_texture(2, geometry_normals_output.output);
        renderer::bind_texture(3, geometry_rough_metal_ao_output.output);
        renderer::bind_texture(4, environment_pass_input.render_pass->get_output("irradiance_output").output);
        renderer::bind_texture(5, environment_pass_input.render_pass->get_output("prefilter_output").output);
        renderer::bind_texture(6, environment_pass_input.render_pass->get_output("brdf_output").output);
        renderer::bind_texture(7, ambient_occlusion_pass_input.render_pass->get_output("ssao_blur_output").output);

        // Render screen quad mesh.
        renderer::submit_mesh(m_screen_mesh);

        m_lighting_shader->un_bind();
        m_fbo->un_bind();
    }

    void lighting_pass::set_outputs()
    {
        render_pass_output lighting_output;
        lighting_output.name = "lighting_output";
        lighting_output.output = m_fbo->get_attachment_id(0);
        add_output(lighting_output);
    }

    void lighting_pass::update_lights()
    {
        collect_lights();
        update_lights_buffer();
        m_lights_ubo->bind(1);
    }

    void lighting_pass::initialize_lights_data()
    {
        for (int i = 0; i < 4; i++)
        {
            m_point_lights[i].color = glm::vec4(0);
            m_point_lights[i].position = glm::vec4(0);
        }

        for (int i = 0; i < 4; i++)
        {
            m_spot_lights[i].color = glm::vec4(0);
            m_spot_lights[i].position = glm::vec4(0);
            m_spot_lights[i].direction = glm::vec4(0);
            m_spot_lights[i].inner_cone_angle = 0;
            m_spot_lights[i].outer_cone_angle = 0;
        }

        m_directional_light.color = glm::vec4(0);
        m_directional_light.direction = glm::vec4(0);
    }

    void lighting_pass::collect_lights()
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        const auto& registry = current_scene->get_registry();

        // Point lights.
        const auto& point_view = registry->view<scene::point_light_component>();
        int point_lights_count = 0;
        for (auto [actor, point_light_component] : point_view.each())
        {
            auto& point_light = point_light_component.get_point_light();

            point_light_data point_light_data;
            point_light_data.position = glm::vec4(point_light->get_position(), 1.0);
            point_light_data.color = glm::vec4(point_light->get_color(), point_light->get_intensity());

            m_point_lights[point_lights_count++] = point_light_data;
        }

        // Spot lights.
        const auto& spot_view = registry->view<scene::spot_light_component>();
        int spot_lights_count = 0;
        for (auto [actor, spot_light_component] : spot_view.each())
        {
            auto& spot_light = spot_light_component.get_spot_light();

            spot_light_data spot_light_data;
            spot_light_data.position = glm::vec4(spot_light->get_position(), 1.0);
            spot_light_data.color = glm::vec4(spot_light->get_color(), spot_light->get_intensity());
            spot_light_data.direction = glm::vec4(spot_light->get_direction(), 1.0);
            spot_light_data.inner_cone_angle = spot_light->get_inner_cone_angle();
            spot_light_data.outer_cone_angle = spot_light->get_outer_cone_angle();

            m_spot_lights[spot_lights_count++] = spot_light_data;
        }

        // Directional lights.
        const auto& directional_view = registry->view<scene::directional_light_component>();
        for (auto [actor, directional_light_component] : directional_view.each())
        {
            auto& directional_light = directional_light_component.get_directional_light();

            m_directional_light.direction = glm::vec4(directional_light->get_direction(), 1.0);
            m_directional_light.color =
                glm::vec4(directional_light->get_color(), directional_light->get_intensity());
        }
    }

    void lighting_pass::update_lights_buffer()
    {
        m_lights_ubo->set_data(&m_point_lights, sizeof(m_point_lights));
        m_lights_ubo->set_data(&m_spot_lights, sizeof(m_spot_lights), sizeof(m_point_lights));
        m_lights_ubo->set_data(&m_directional_light, sizeof(m_directional_light),
                               sizeof(m_point_lights) + sizeof(m_spot_lights));
    }
}
