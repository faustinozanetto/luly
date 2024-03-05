#pragma once

#include "physics_error_callback.h"
#include "utils/singleton.h"

#include <PxPhysicsAPI.h>

namespace luly::physics
{
    class physics_world : public singleton<physics_world>
    {
    public:
        physics_world();
        ~physics_world() override;

        /* Getters */
        physx::PxPhysics* get_physics() const { return m_physics; }
        physx::PxDefaultCpuDispatcher* get_dispatcher() const { return m_dispatcher; }
        bool get_simulate() const { return m_simulate; }

        /* Setters */
        void set_simulate(bool simulate) { m_simulate = simulate; }

        void on_update() const;
        void sync_transforms() const;

    private:
        void initialize_physx();
        void cleanup();

        physics_error_callback m_error_callback;
        physx::PxDefaultAllocator m_default_allocator_callback;
        physx::PxDefaultCpuDispatcher* m_dispatcher;
        physx::PxTolerancesScale m_toleranceScale;
        physx::PxFoundation* m_foundation;
        physx::PxPhysics* m_physics;
        physx::PxPvd* m_pvd;

        bool m_simulate;
    };
}
