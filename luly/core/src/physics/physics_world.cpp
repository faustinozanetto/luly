#include "lypch.h"
#include "physics_world.h"

#include "physics_utils.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "time/app_time.h"

namespace luly::physics
{
    physics_world::physics_world()
    {
        m_simulate = false;
        initialize_physx();
    }

    physics_world::~physics_world()
    {
        cleanup();
    }

    void physics_world::on_update() const
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        if (!current_scene) return;

        physx::PxScene* current_scene_physx = current_scene->get_physx_scene();
        if (current_scene_physx && m_simulate)
        {
            current_scene_physx->simulate(app_time::get_delta_time());
            current_scene_physx->fetchResults(true);
        }
        /*
        if (m_scene && m_simulate)
        {
            m_scene->simulate(app_time::get_delta_time());
            m_scene->fetchResults(true);
        }*/
    }

    void physics_world::sync_transforms()
    {
        const auto& current_scene = scene::scene_manager::get().get_current_scene();
        if (!current_scene && !m_simulate) return;

        const auto& scene_view = current_scene->get_registry()->view<
            scene::transform_component, scene::physics_dynamic_actor_component>();
        for (auto&& [actor, transform_comp, physics_dynamic_actor_comp] : scene_view.each())
        {
            const std::shared_ptr<math::transform>& transform = transform_comp.get_transform();

            if (physics_dynamic_actor_comp.get_dynamic_actor()->get_is_kinematic())
            {
                physx::PxTransform physx_transform = physics_utils::create_transform_from_glm(
                    transform->get_location(), transform->get_rotation());
                physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->setKinematicTarget(
                    physx_transform);
            }
            else
            {
                transform->set_location(physics_utils::convert_physx_vec3_to_glm(
                    physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().
                                               p));
                transform->set_rotation(physics_utils::convert_physx_quat_to_glm(
                    physics_dynamic_actor_comp.get_dynamic_actor()->get_physx_rigid_dynamic_actor()->getGlobalPose().
                                               q));
            }
        }
    }

    void physics_world::initialize_physx()
    {
        LY_TRACE("Started initializing PhysX world...");

        // Create the foundation
        m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_default_allocator_callback, m_error_callback);
        LY_ASSERT_MSG(m_foundation, "An error occurred while creating PhysX Foundation!");

#ifdef LY_DEBUG
        // Create pvd
        m_pvd = PxCreatePvd(*m_foundation);
        LY_ASSERT_MSG(m_pvd, "An error occurred while creating PhysX PVD!");

        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
#endif
        // Create the physics instance
        m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), true, m_pvd);
        LY_ASSERT_MSG(m_physics, "An error occurred while creating PhysX Instance!")

        PxInitExtensions(*m_physics, m_pvd);

        // Create the default CPU dispatcher
        m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        LY_ASSERT_MSG(m_dispatcher, "An error occurred while creating PhysX Dispatcher!")

        LY_TRACE("PhysX world initialized successfully!");
    }

    void physics_world::cleanup()
    {
        /*
        if (m_scene)
        {
            m_scene->release();
            m_scene = nullptr;
        }
        */
        if (m_dispatcher)
        {
            m_dispatcher->release();
            m_dispatcher = nullptr;
        }
        PxCloseExtensions();
        if (m_physics)
        {
            m_physics->release();
            m_physics = nullptr;
        }
        if (m_foundation)
        {
            m_foundation->release();
            m_foundation = nullptr;
        }
    }
}
