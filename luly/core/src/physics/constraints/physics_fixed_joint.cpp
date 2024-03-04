#include "lypch.h"
#include "physics_fixed_joint.h"

#include "physics/physics_world.h"

namespace luly::physics
{
    physics_fixed_joint::physics_fixed_joint(const std::shared_ptr<physics_actor>& physics_actor_a,
                                             const physx::PxTransform& t0,
                                             const std::shared_ptr<physics_actor>& physics_actor_b,
                                             const physx::PxTransform& t1) : physics_joint(
        physics_actor_a, physics_actor_b)
    {
        // Create the fixed joint
        physx::PxPhysics* physics = physics_world::get().get_physics();
        physx::PxRigidActor* rigid_actor_a = m_physics_actor_a ? m_physics_actor_a->get_physx_rigid_actor() : nullptr;
        physx::PxRigidActor* rigid_actor_b = m_physics_actor_b ? m_physics_actor_b->get_physx_rigid_actor() : nullptr;

        m_physx_fixed_joint = PxFixedJointCreate(*physics, rigid_actor_a, t0, rigid_actor_b, t1);
        LY_ASSERT_MSG(m_physx_fixed_joint, "Failed to create PhysX fixed joint!")
    }

    physics_fixed_joint::~physics_fixed_joint()
    {
        physics_fixed_joint::release_joint();
    }

    void physics_fixed_joint::release_joint()
    {
        if (m_physx_fixed_joint)
        {
            m_physx_fixed_joint->release();
            m_physx_fixed_joint = nullptr;
        }
    }

    void physics_fixed_joint::set_projection_linear_tolerance(float distance) const
    {
        m_physx_fixed_joint->setProjectionLinearTolerance(distance);
    }

    float physics_fixed_joint::get_projection_linear_tolerance() const
    {
        return m_physx_fixed_joint->getProjectionLinearTolerance();
    }

    void physics_fixed_joint::set_projection_angular_tolerance(float tolerance) const
    {
        m_physx_fixed_joint->setProjectionAngularTolerance(tolerance);
    }

    float physics_fixed_joint::get_projection_angular_tolerance() const
    {
        return m_physx_fixed_joint->getProjectionAngularTolerance();
    }
}
