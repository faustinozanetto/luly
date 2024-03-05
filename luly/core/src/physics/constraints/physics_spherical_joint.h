#pragma once

#include "physics/constraints/physics_joint.h"

#include <glm/ext/scalar_constants.hpp>

namespace luly::physics
{
    class physics_spherical_joint : public physics_joint
    {
    public:
        physics_spherical_joint(const std::shared_ptr<physics_actor>& physics_actor_a, const physx::PxTransform& t0,
                                const std::shared_ptr<physics_actor>& physics_actor_b, const physx::PxTransform& t1);
        ~physics_spherical_joint() override;

        /* Getters */
        physx::PxJointLimitCone get_limit_cone() const;
        float get_swing_y_angle() const;
        float get_swing_z_angle() const;
        float get_projection_linear_tolerance() const;
        physx::PxSphericalJointFlags get_spherical_joint_flags() const;

        /* Setters */
        void set_limit_cone(float y_limit = glm::pi<float>() / 2, float z_limit = glm::pi<float>() / 2) const;
        void set_projection_linear_tolerance(float tolerance) const;
        void set_spherical_joint_flags(physx::PxSphericalJointFlags flags) const;
        void set_spherical_joint_flag(physx::PxSphericalJointFlag::Enum flag, bool value) const;

    protected:
        void release_joint() override;

    private:
        physx::PxSphericalJoint* m_physx_spherical_joint;
    };
}
