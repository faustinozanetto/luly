﻿#pragma once

#include "physics/actors/physics_actor.h"

#include <PxPhysicsAPI.h>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

namespace luly::physics
{
    class physics_dynamic_actor : public physics_actor
    {
    public:
        physics_dynamic_actor(physx::PxRigidDynamic* rigid_dynamic);
        physics_dynamic_actor(const glm::vec3& location = glm::vec3(0.0f),
                              const glm::quat& rotation = glm::quat(1.0, 0.0f, 0.0f, 0.0f), float mass = 1.0f);
        ~physics_dynamic_actor() override;

        /* Getters */
        physx::PxRigidDynamic* get_physx_rigid_dynamic_actor() const { return m_rigid_dynamic; }
        physx::PxRigidActor* get_physx_rigid_actor() override { return m_rigid_dynamic; }
        float get_mass() const { return m_mass; }
        float get_linear_damping() const;
        float get_angular_damping() const;
        glm::vec3 get_linear_velocity() const;
        glm::vec3 get_angular_velocity() const;
        glm::vec3 get_center_of_mass() const;
        bool get_is_kinematic() const;

        /* Setters */
        void set_mass(float mass);
        void set_linear_damping(float linear_damping);
        void set_angular_damping(float angular_damping);
        void set_linear_velocity(const glm::vec3& linear_velocity);
        void set_angular_velocity(const glm::vec3& angular_velocity);
        void set_center_of_mass(const glm::vec3& center_of_mass);
        void set_kinematic(bool is_kinematic);

        /* Functions */
        void initialize(scene::scene* scene) override;

    protected:
        void attach_collision_shapes() override;

    private:
        physx::PxRigidDynamic* m_rigid_dynamic;
        float m_mass;
    };
}
