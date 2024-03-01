#include "lypch.h"
#include "physics_world.h"

#include "physics_utils.h"

namespace luly::physics
{
    physics_world::physics_world(const glm::vec3& gravity)
    {
        m_gravity = gravity;
        initialize_physx();
    }

    physics_world::~physics_world()
    {
        cleanup();
    }

    void physics_world::initialize_physx()
    {
        LY_TRACE("Started initializing PhysX world...");

        // Create the foundation
        m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_default_allocator_callback, m_error_callback);
        LY_ASSERT_MSG(m_foundation, "An error occurred while creating PhysX Foundation!")

        // Create pvd
        m_pvd = PxCreatePvd(*m_foundation);
        physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
        m_pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

        m_toleranceScale.length = 100;
        m_toleranceScale.speed = 981;

        // Create the physics instance
        m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, m_toleranceScale, true, m_pvd);
        LY_ASSERT_MSG(m_physics, "An error occurred while creating PhysX Instance!")

        // Create the scene
        physx::PxSceneDesc scene_desc(m_physics->getTolerancesScale());
        scene_desc.gravity = physics_utils::convert_glm_vec3_to_physx(m_gravity);
        m_dispatcher = physx::PxDefaultCpuDispatcherCreate(2);
        LY_ASSERT_MSG(m_dispatcher, "An error occurred while creating PhysX Dispatcher!")

        scene_desc.cpuDispatcher = m_dispatcher;
        scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;
        m_scene = m_physics->createScene(scene_desc);
        LY_ASSERT_MSG(m_scene, "An error occurred while creating PhysX Scene!")

        if (physx::PxPvdSceneClient* pvd_client = m_scene->getScenePvdClient())
        {
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
            pvd_client->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
        }

        m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LOCAL_FRAMES, 1.0f);
        m_scene->setVisualizationParameter(physx::PxVisualizationParameter::eJOINT_LIMITS, 1.0f);

        LY_TRACE("PhysX world initialized successfully!");
    }

    void physics_world::cleanup()
    {
        if (m_scene)
        {
            m_scene->release();
            m_scene = nullptr;
        }
        if (m_dispatcher)
        {
            m_dispatcher->release();
            m_dispatcher = nullptr;
        }
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
