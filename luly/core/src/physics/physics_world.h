#pragma once

#include "physics_error_callback.h"
#include "utils/singleton.h"

#include <PxPhysicsAPI.h>

#include <glm/glm.hpp>

namespace luly::physics
{
    class physics_world : public singleton<physics_world>
    {
    public:
        physics_world();
        ~physics_world() override;

        /* Getters */
        physx::PxPhysics* get_physics() { return m_physics; }
        physx::PxDefaultCpuDispatcher* get_dispatcher() { return  m_dispatcher;}
       // physx::PxScene* get_scene() { return m_scene; }
        bool get_simulate() const { return m_simulate; }

        /* Setters */
        void set_simulate(bool simulate) { m_simulate = simulate; }

        void on_update() const;
        void sync_transforms();

    private:
        void initialize_physx();
        void cleanup();

        physics_error_callback m_error_callback;
        physx::PxDefaultAllocator m_default_allocator_callback;
        physx::PxDefaultCpuDispatcher* m_dispatcher;
        physx::PxTolerancesScale m_toleranceScale;
        physx::PxFoundation* m_foundation;
        physx::PxPhysics* m_physics;
        //physx::PxScene* m_scene;
        physx::PxPvd* m_pvd;

        bool m_simulate;
    };
}
