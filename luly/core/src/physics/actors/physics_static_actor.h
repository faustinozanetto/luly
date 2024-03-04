#pragma once

#include "physics/actors/physics_actor.h"

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

namespace luly::physics
{
    class physics_static_actor : public physics_actor
    {
    public:
        physics_static_actor(physx::PxRigidStatic* rigid_static);
        physics_static_actor(const glm::vec3& location = glm::vec3(0.0f),
                             const glm::quat& rotation = glm::quat(1.0, 0.0f, 0.0f, 0.0f));
        ~physics_static_actor() override;

        /* Getters */
        physx::PxRigidStatic* get_physx_rigid_static() const { return m_rigid_static; }
        physx::PxRigidActor* get_physx_rigid_actor() override { return m_rigid_static; }

        /* Functions */
        void initialize(scene::scene* scene) override;

    protected:
        void attach_collision_shapes() override;

    private:
        physx::PxRigidStatic* m_rigid_static;
    };
}
