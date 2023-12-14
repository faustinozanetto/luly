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
            sizeof(m_lights_data.point_lights) + sizeof(m_lights_data.spot_lights) + sizeof(m_lights_data.
                directional_light), LIGHTS_UBO_LOCATION);
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
            m_lights_data.point_lights[i].color = glm::vec3(0.0f);
            m_lights_data.point_lights[i].intensity = 0.0f;
            //  m_lights_data.point_lights[i].pad1 = glm::vec2(0.0f);
            m_lights_data.point_lights[i].position = glm::vec3(0.0f);
            m_lights_data.point_lights[i].constant_factor = 0.0f;
            m_lights_data.point_lights[i].linear_factor = 0.0f;
            m_lights_data.point_lights[i].quadratic_factor = 0.0f;
        }

        for (int i = 0; i < MAX_SPOT_LIGHTS; i++)
        {
            m_lights_data.spot_lights[i].color = glm::vec3(0.0f);
            m_lights_data.spot_lights[i].intensity = 0.0f;
            //  m_lights_data.spot_lights[i].pad1 = glm::vec2(0.0f);
            m_lights_data.spot_lights[i].position = glm::vec3(0.0f);
            // m_lights_data.spot_lights[i].pad2 = glm::vec2(0.0f);
            m_lights_data.spot_lights[i].direction = glm::vec3(0.0f);
            // m_lights_data.spot_lights[i].pad3 = glm::vec2(0.0f);
            m_lights_data.spot_lights[i].angles = glm::vec2(0.0f);
        }

        m_lights_data.directional_light.color = glm::vec3(0.0f);
        m_lights_data.directional_light.intensity = 0.0f;
        //m_lights_data.directional_light.pad1 = glm::vec2(0.0f);
        m_lights_data.directional_light.direction = glm::vec3(0.0f);
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

            m_lights_data.point_lights[point_lights_count].position = point_light->get_position();
            m_lights_data.point_lights[point_lights_count].color = point_light->get_color();
            m_lights_data.point_lights[point_lights_count].intensity = point_light->get_intensity();
            m_lights_data.point_lights[point_lights_count].constant_factor = point_light->get_constant_factor();
            m_lights_data.point_lights[point_lights_count].linear_factor = point_light->get_linear_factor();
            m_lights_data.point_lights[point_lights_count].quadratic_factor = point_light->get_quadratic_factor();

            point_lights_count++;
        }

        // Spot lights.
        const auto& spot_view = registry->view<scene::spot_light_component>();
        int spot_lights_count = 0;
        for (auto [actor, spot_light_component] : spot_view.each())
        {
            const std::shared_ptr<spot_light>& spot_light = spot_light_component.get_spot_light();

            m_lights_data.spot_lights[spot_lights_count].position = spot_light->get_position();
            m_lights_data.spot_lights[spot_lights_count].color = spot_light->get_color();
            m_lights_data.spot_lights[spot_lights_count].intensity = spot_light->get_intensity();
            m_lights_data.spot_lights[spot_lights_count].direction = spot_light->get_direction();
            m_lights_data.spot_lights[spot_lights_count].angles = glm::vec2(
                spot_light->get_inner_cone_angle(), spot_light->get_outer_cone_angle());

            spot_lights_count++;
        }

        // Directional lights.
        const auto& directional_view = registry->view<scene::directional_light_component>();
        for (auto [actor, directional_light_component] : directional_view.each())
        {
            const std::shared_ptr<directional_light>& directional_light = directional_light_component.
                get_directional_light();

            m_lights_data.directional_light.direction = directional_light->get_direction();
            m_lights_data.directional_light.color = directional_light->get_color();
            m_lights_data.directional_light.intensity = directional_light->get_intensity();
        }
    }

    void lights_manager::update_lights_buffer() const
    {
        LY_PROFILE_FUNCTION;
        m_lights_ubo->set_data(&m_lights_data.point_lights, sizeof(m_lights_data.point_lights));
        m_lights_ubo->set_data(&m_lights_data.spot_lights, sizeof(m_lights_data.spot_lights),
                               sizeof(m_lights_data.point_lights));
        m_lights_ubo->set_data(&m_lights_data.directional_light, sizeof(m_lights_data.directional_light),
                               sizeof(m_lights_data.point_lights) + sizeof(m_lights_data.spot_lights));
    }
}
