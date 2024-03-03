#pragma once


#include "physics/collision_shapes/physics_sphere_collision.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class physics_sphere_collision_shape_component : public base_component
    {
    public:
        physics_sphere_collision_shape_component();

        physics_sphere_collision_shape_component(
            const std::shared_ptr<physics::physics_sphere_collision>& sphere_collision) : m_sphere_collision(
            sphere_collision)
        {
        }

        ~physics_sphere_collision_shape_component() override = default;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<physics::physics_sphere_collision>& get_sphere_collision() const
        {
            return m_sphere_collision;
        }

        /* Setters */
        void set_sphere_collision(const std::shared_ptr<physics::physics_sphere_collision>& sphere_collision)
        {
            m_sphere_collision = sphere_collision;
        }

    private:
        std::shared_ptr<physics::physics_sphere_collision> m_sphere_collision;
    };
}
