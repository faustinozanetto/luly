#include "lypch.h"
#include "physics_box_collision.h"

#include "physics/physics_utils.h"
#include "physics/physics_world.h"
#include "physics/actors/physics_actor.h"

namespace luly::physics
{
    physics_box_collision::physics_box_collision(const std::shared_ptr<physics_actor>& physics_actor,
                                                 const std::shared_ptr<physics_material>& physics_material,
                                                 const glm::vec3& half_extents) : physics_collision_shape(physics_actor,
        physics_material)
    {
        m_half_extents = half_extents;
        m_shape = physics_world::get().get_physics()->createShape(
            physx::PxBoxGeometry(physics_utils::convert_glm_vec3_to_physx(half_extents)),
            *m_physics_material->get_physx_material());
    }

    physics_box_collision::~physics_box_collision()
    {
        if (m_shape)
        {
            // Release the existing shape
            m_physics_actor->get_physx_rigid_actor()->detachShape(*m_shape);
            m_shape = nullptr;
        }
    }

    physx::PxShape* physics_box_collision::get_physx_shape() const
    {
        return m_shape;
    }

    void physics_box_collision::set_half_extents(const glm::vec3& half_extents)
    {
        m_half_extents = half_extents;

        // Recreate the PxShape with the new half extents
        if (m_shape)
        {
            // Release the existing shape
            m_physics_actor->get_physx_rigid_actor()->detachShape(*m_shape);
            m_shape = nullptr;
        }

        // Recreate the shape using the new half extents
        m_shape = physics_world::get().get_physics()->createShape(
            physx::PxBoxGeometry(physics_utils::convert_glm_vec3_to_physx(m_half_extents)),
            *m_physics_material->get_physx_material());
        m_physics_actor->get_physx_rigid_actor()->attachShape(*m_shape);
    }
}
