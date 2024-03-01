#pragma once


#include "physics/collision_shapes/physics_plane_collision.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class physics_plane_collision_shape_component : public base_component
    {
    public:
        physics_plane_collision_shape_component();

        physics_plane_collision_shape_component(
            const std::shared_ptr<physics::physics_plane_collision>& plane_collision) : m_plane_collision(plane_collision)
        {
        }

        ~physics_plane_collision_shape_component() override = default;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<physics::physics_plane_collision>& get_plane_collision() const { return m_plane_collision; }

        /* Setters */
        void set_plane_collision(const std::shared_ptr<physics::physics_plane_collision>& plane_collision)
        {
            m_plane_collision = plane_collision;
        }

    private:
        std::shared_ptr<physics::physics_plane_collision> m_plane_collision;
    };
}
