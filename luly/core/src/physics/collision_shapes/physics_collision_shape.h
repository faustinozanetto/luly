#pragma once

#include "physics/physics_material.h"

#include <PxPhysicsAPI.h>

namespace luly::physics
{
    class physics_collision_shape
    {
    public:
        physics_collision_shape(const std::shared_ptr<class physics_actor>& physics_actor,
                                const std::shared_ptr<physics_material>& physics_material) :
            m_physics_material(
                physics_material), m_physics_actor(physics_actor)
        {
        }

        virtual ~physics_collision_shape() = default;

        /* Getters */
        const std::shared_ptr<physics_material>& get_physics_material() const { return m_physics_material; }
        const std::shared_ptr<class physics_actor>& get_physics_actor() const { return m_physics_actor; }
        virtual physx::PxShape* get_physx_shape() const = 0;

    protected:
        std::shared_ptr<physics_material> m_physics_material;
        std::shared_ptr<class physics_actor> m_physics_actor;
    };
}
