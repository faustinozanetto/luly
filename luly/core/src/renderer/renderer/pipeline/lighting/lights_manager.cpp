#include "lypch.h"
#include "lights_manager.h"

#include "renderer/lights/directional_light.h"
#include "renderer/lights/point_light.h"
#include "renderer/lights/spot_light.h"
#include "renderer/renderer/renderer.h"
#include "scene/scene.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/lights/spot_light_component.h"

namespace luly::renderer
{
    lights_manager::lights_manager()
    {
        // Setup lights uniform buffer object.
        initialize_data();
        m_lights_ubo = std::make_shared<uniform_buffer_object>(
            sizeof(m_point_lights) + sizeof(m_spot_lights) + sizeof(m_directional_light) + sizeof(m_lights),
            LIGHTS_UBO_LOCATION);
    }

    void lights_manager::update_lights()
    {
        LY_PROFILE_FUNCTION;
        collect_lights();
        update_lights_buffer();
    }

    void lights_manager::initialize_data()
    {
        LY_PROFILE_FUNCTION;
        for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        {
            m_point_lights[i].color = glm::vec4(0.0f);
            m_point_lights[i].position = glm::vec4(0.0f);
            m_point_lights[i].parameters = glm::vec4(0.0f);
        }

        for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        {
            m_spot_lights[i].color = glm::vec4(0.0f);
            m_spot_lights[i].position = glm::vec4(0.0f);
            m_spot_lights[i].direction = glm::vec4(0.0f);
            m_spot_lights[i].parameters = glm::vec4(0.0f);
        }

        m_directional_light.color = glm::vec4(0.0f);
        m_directional_light.direction = glm::vec4(0.0f);
        m_directional_light.parameters = glm::vec4(0.0f);

        m_lights.parameters = glm::ivec4(0);
    }

    void lights_manager::collect_lights()
    {
        LY_PROFILE_FUNCTION;
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        const auto& registry = current_scene->get_registry();

        // Point lights.
        const auto& point_view = registry->view<scene::point_light_component>();
        int point_lights_count = 0;
        for (auto [actor, point_light_component] : point_view.each())
        {
            const std::shared_ptr<point_light>& point_light = point_light_component.get_point_light();

            m_point_lights[point_lights_count].position = glm::vec4(point_light->get_position(), 1.0);
            m_point_lights[point_lights_count].color = glm::vec4(point_light->get_color(), 1.0);
            m_point_lights[point_lights_count].parameters = glm::vec4(point_light->get_intensity()
                                                                      , point_light->get_constant_factor(),
                                                                      point_light->get_linear_factor(),
                                                                      point_light->get_quadratic_factor());

            point_lights_count++;
        }

        // Spot lights.
        const auto& spot_view = registry->view<scene::spot_light_component>();
        int spot_lights_count = 0;
        for (auto [actor, spot_light_component] : spot_view.each())
        {
            const std::shared_ptr<spot_light>& spot_light = spot_light_component.get_spot_light();

            m_spot_lights[spot_lights_count].position = glm::vec4(spot_light->get_position(), 1.0);
            m_spot_lights[spot_lights_count].color = glm::vec4(spot_light->get_color(), 1.0);
            m_spot_lights[spot_lights_count].direction = glm::vec4(spot_light->get_direction(), 1.0);
            m_spot_lights[spot_lights_count].parameters = glm::vec4(
                spot_light->get_inner_cone_angle(), spot_light->get_outer_cone_angle(), spot_light->get_intensity(),
                0.0);

            spot_lights_count++;
        }

        // Directional lights.
        const auto& directional_view = registry->view<scene::directional_light_component>();
        for (auto [actor, directional_light_component] : directional_view.each())
        {
            const std::shared_ptr<directional_light>& directional_light = directional_light_component.
                get_directional_light();

            m_directional_light.direction = glm::vec4(directional_light->get_direction(), 1.0);
            m_directional_light.color = glm::vec4(directional_light->get_color(), 1.0);
            m_directional_light.parameters = glm::vec4(directional_light->get_intensity(), 0.0, 0.0, 0.0);
        }

        m_lights.parameters = glm::ivec4(point_lights_count, spot_lights_count, 0, 0);
    }

    void lights_manager::update_lights_buffer() const
    {
        LY_PROFILE_FUNCTION;
        m_lights_ubo->set_data(&m_point_lights, sizeof(m_point_lights));
        m_lights_ubo->set_data(&m_spot_lights, sizeof(m_spot_lights),
                               sizeof(m_point_lights));
        m_lights_ubo->set_data(&m_directional_light, sizeof(m_directional_light),
                               sizeof(m_point_lights) + sizeof(m_spot_lights));
        m_lights_ubo->set_data(&m_lights, sizeof(m_lights),
                               sizeof(m_point_lights) + sizeof(m_spot_lights) + sizeof(m_directional_light));
    }
}
