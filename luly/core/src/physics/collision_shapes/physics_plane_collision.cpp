#include "lypch.h"
#include "physics_plane_collision.h"

#include "physics/physics_world.h"
#include "physics/actors/physics_actor.h"

namespace luly::physics
{
    physics_plane_collision::physics_plane_collision(const std::shared_ptr<physics_actor>& physics_actor,
                                                     const std::shared_ptr<physics_material>& physics_material) :
        physics_collision_shape(physics_actor, physics_material)
    {
        m_shape = physics_world::get().get_physics()->createShape(physx::PxPlaneGeometry(),
                                                                  *m_physics_material->get_physx_material());
    }

    physics_plane_collision::~physics_plane_collision()
    {
        if (m_shape)
        {
            // Release the existing shape
            m_physics_actor->get_physx_rigid_actor()->detachShape(*m_shape);
            m_shape = nullptr;
        }
    }

    physx::PxShape* physics_plane_collision::get_physx_shape() const
    {
        return m_shape;
    }
}
