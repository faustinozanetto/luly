#include "lypch.h"
#include "scene.h"

#include "actor/components/name_component.h"
#include "actor/components/transform_component.h"
#include "actor/components/base_component.h"
#include "actor/components/lights/directional_light_component.h"
#include "actor/components/lights/point_light_component.h"
#include "actor/components/lights/spot_light_component.h"
#include "actor/components/rendering/skybox_component.h"

#include "renderer/scene/scene_renderer.h"

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

    void scene::delete_actor(entt::entity handle)
    {
        if (!m_actors_map.contains(handle))
        {
            LY_WARN("Tried to delete actor with handle: {}, that does not exist!", static_cast<uint32_t>(handle));
            return;
        }

        if (m_pending_delete_entities.contains(handle))
        {
            LY_WARN("Tried to add actor with handle: {}, to delete that already has been added!",
                    static_cast<uint32_t>(handle));
            return;
        }

        m_actors_map.erase(handle);
        m_pending_delete_entities.insert(handle);
    }

    void scene::on_update(float delta_time)
    {
        update_lights();
    }

    void scene::handle_delete_entities()
    {
        for (entt::entity entity : m_pending_delete_entities)
        {
            if (!m_actors_registry->valid(entity))
                continue;

            m_actors_registry->destroy(entity);
            LY_TRACE("Deleting entity with handle: {}!", static_cast<uint32_t>(entity));
        }
    }

    std::vector<directional_light_component> scene::get_directional_light() const
    {
        std::vector<directional_light_component> directional_lights;
        const auto& view = m_actors_registry->view<directional_light_component>();
        for (auto [actor, directional_light_component] : view.each())
        {
            directional_lights.push_back(directional_light_component);
        }
        return directional_lights;
    }

    std::vector<point_light_component> scene::get_point_lights() const
    {
        std::vector<point_light_component> point_lights;
        const auto& view = m_actors_registry->view<point_light_component>();
        for (auto [actor, point_light_component] : view.each())
        {
            point_lights.push_back(point_light_component);
        }
        return point_lights;
    }

    const std::shared_ptr<scene_actor>& scene::get_skybox_actor() const
    {
        const auto& view = m_actors_registry->view<skybox_component>();
        for (auto [actor, skybox_component] : view.each())
        {
            return m_actors_map.at(actor);
        }
        LY_ASSERT_MSG(false, "Could not find actor with skybox component")
        return nullptr;
    }

    void scene::update_lights()
    {
        // Update point lights
        const auto& point_view = m_actors_registry->view<transform_component, point_light_component>();
        for (auto [actor, transform_component, point_light_component] : point_view.each())
        {
            auto& transform = transform_component.get_transform();
            auto& point_light = point_light_component.get_point_light();

            point_light->set_position(transform->get_location());
        }

        // Update spot lights
        const auto& spot_view = m_actors_registry->view<transform_component, spot_light_component>();
        for (auto [actor, transform_component, spot_light_component] : spot_view.each())
        {
            auto& transform = transform_component.get_transform();
            auto& spot_light = spot_light_component.get_spot_light();

            spot_light->set_position(transform->get_location());
        }
    }
}
