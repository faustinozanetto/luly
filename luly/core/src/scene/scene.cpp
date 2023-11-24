#include "lypch.h"
#include "scene.h"

#include "actor/components/name_component.h"
#include "actor/components/transform_component.h"

#include <logging/log.h>

#include "actor/components/lights/point_light_component.h"

namespace luly::scene
{
    scene::scene(const std::string& name)
    {
        m_name = name;

        LY_TRACE("Started creating scene...");
        LY_TRACE("  - Name: '{}'", m_name);

        m_actors_registry = std::make_unique<entt::registry>();
        m_camera_manager = std::make_shared<renderer::camera_manager>();

        LY_TRACE("Scene created successfully!");
    }

    scene::~scene()
    {
    }

    const std::shared_ptr<scene_actor>& scene::get_actor(entt::entity entity)
    {
        const auto iterator = m_actors_map.find(entity);
        if (iterator == m_actors_map.end())
            LY_ASSERT_MSG(m_actors_map.find(entity) != m_actors_map.end(), "Actor does not belong to the registry!")

        return iterator->second;
    }

    std::shared_ptr<scene_actor> scene::create_actor(const std::string& name)
    {
        entt::entity actor_handle = m_actors_registry->create();
        const auto& actor = std::make_shared<scene_actor>(actor_handle, this);
        actor->add_component<name_component>(name);
        actor->add_component<transform_component>();

        m_actors_map.insert({actor_handle, actor});

        return actor;
    }

    void scene::on_update(float delta_time)
    {
        update_lights();
    }

    void scene::update_lights()
    {
        auto& view = m_actors_registry->view<transform_component, point_light_component>();
        for (auto [actor, transform_component, point_light_component] : view.each())
        {
            auto& transform = transform_component.get_transform();
            auto& point_light = point_light_component.get_point_light();

            point_light->set_position(transform->get_location());
        }
    }
}
