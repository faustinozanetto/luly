#include "lypch.h"
#include "lighting_pass.h"

#include "application/application.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"

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

        // Create lights ubo.
        m_lights_ubo = std::make_shared<uniform_buffer_object>(sizeof(m_directional_light) + sizeof(m_point_lights), 1);

        // Create lighting shader.
        m_lighting_shader = shader_factory::create_shader_from_file("assets/shaders/lighting_pass_shader.lsh");

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();

        for (int i = 0; i < 10; i++)
        {
            m_point_lights[i].color = glm::vec4(0);
            m_point_lights[i].position = glm::vec4(0);
        }
    }

    void lighting_pass::execute()
    {
        m_fbo->bind();
        renderer::clear_screen();
        m_lighting_shader->bind();

        render_pass_input geometry_pass_input = m_inputs.at("geometry_pass_input");

        // Bind geometry pass outputs.
        renderer::bind_texture(0, geometry_pass_input.render_pass->get_frame_buffer()->get_attachment_id(0));
        renderer::bind_texture(1, geometry_pass_input.render_pass->get_frame_buffer()->get_attachment_id(1));
        renderer::bind_texture(2, geometry_pass_input.render_pass->get_frame_buffer()->get_attachment_id(2));
        renderer::bind_texture(3, geometry_pass_input.render_pass->get_frame_buffer()->get_attachment_id(3));

        renderer::submit_mesh(m_screen_mesh);

        m_lighting_shader->un_bind();
        m_fbo->un_bind();
    }

    void lighting_pass::update_lights()
    {
        collect_lights();
        update_lights_buffer();
        m_lights_ubo->bind(1);
    }

    void lighting_pass::collect_lights()
    {
        auto& current_scene = core::application::get().get_scene_manager()->get_current_scene();
        const auto& registry = current_scene->get_registry();

        // Point lights.
        const auto& point_view = registry->view<scene::point_light_component>();
        int point_light_index = 0;
        for (auto [actor, point_light_component] : point_view.each())
        {
            auto& point_light = point_light_component.get_point_light();

            point_light_data point_light_data;
            point_light_data.position = glm::vec4(point_light->get_position(), 1.0);
            point_light_data.color = glm::vec4(point_light->get_color(), point_light->get_intensity());

            m_point_lights[point_light_index++] = point_light_data;
        }

        // Directional lights.
        const auto& directional_view = registry->view<scene::directional_light_component>();
        for (auto [actor, directional_light_component] : directional_view.each())
        {
            auto& directional_light = directional_light_component.get_directional_light();

            directional_light_data directional_light_data;
            directional_light_data.direction = glm::vec4(directional_light->get_direction(), 1.0);
            directional_light_data.color =
                glm::vec4(directional_light->get_color(), directional_light->get_intensity());

            m_directional_light = directional_light_data;
        }
    }

    void lighting_pass::update_lights_buffer()
    {
        m_lights_ubo->set_data(&m_point_lights, sizeof(m_point_lights));
        m_lights_ubo->set_data(&m_directional_light, sizeof(m_directional_light), sizeof(m_point_lights));
    }
}
