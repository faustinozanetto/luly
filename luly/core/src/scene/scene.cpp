#include "lypch.h"
#include "scene.h"

#include "actor/components/name_component.h"
#include "actor/components/transform_component.h"
#include "actor/components/base_component.h"
#include "actor/components/lights/directional_light_component.h"
#include "actor/components/lights/point_light_component.h"
#include "actor/components/lights/spot_light_component.h"
#include "actor/components/rendering/skybox_component.h"
#include "physics/physics_utils.h"
#include "physics/physics_world.h"
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

        initialize_physx_scene();

        LY_TRACE("Scene created successfully!");
    }

    scene::~scene()
    {
        if (m_physx_scene)
        {
            m_physx_scene->release();
            m_physx_scene = nullptr;
        }
    }

    const std::shared_ptr<scene_actor>& scene::get_actor(entt::entity entity)
    {
        const auto iterator = m_actors_map.find(entity);
        if (iterator == m_actors_map.end())
            LY_ASSERT_MSG(m_actors_map.find(entity) != m_actors_map.end(), "Actor does not belong to the registry!")

        return iterator->second;
    }

    void scene::set_gravity(const glm::vec3& gravity)
    {
        m_gravity = gravity;
        m_physx_scene->setGravity(physics::physics_utils::convert_glm_vec3_to_physx(m_gravity));
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

        if (m_pending_delete_actors.contains(handle))
        {
            LY_WARN("Tried to add actor with handle: {}, to delete that already has been added!",
                    static_cast<uint32_t>(handle));
            return;
        }

        m_actors_map.erase(handle);
        m_pending_delete_actors.insert(handle);
    }

    void scene::on_update(float delta_time)
    {
        update_lights();
    }

    void scene::handle_delete_entities()
    {
        for (entt::entity entity : m_pending_delete_actors)
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

    void scene::initialize_physx_scene()
    {
        LY_TRACE("Started creating physics scene for scene: '{}'...", m_name);
        physics::physics_world& physics_world = physics::physics_world::get();
        m_gravity = glm::vec3(0.0f, -9.81f, 0.0f);

        // Create the scene
        physx::PxSceneDesc sceneDesc(physics_world.get_physics()->getTolerancesScale());
        sceneDesc.cpuDispatcher = physics_world.get_dispatcher();
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
        sceneDesc.gravity = physics::physics_utils::convert_glm_vec3_to_physx(m_gravity);
        m_physx_scene = physics_world.get_physics()->createScene(sceneDesc);
        LY_ASSERT_MSG(m_physx_scene, "An error occurred while creating PhysX Scene!");
        
        m_physx_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
        m_physx_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

        // Create pvd
#ifdef LY_DEBUG
        if (physx::PxPvdSceneClient* pvd_client = m_physx_scene->getScenePvdClient())
        {
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }
#endif
        LY_TRACE("Physics scene created successfully!");
    }

    void scene::update_lights() const
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
