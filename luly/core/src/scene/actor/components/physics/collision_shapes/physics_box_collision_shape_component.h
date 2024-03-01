#pragma once


#include "physics/collision_shapes/physics_box_collision.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class physics_box_collision_shape_component : public base_component
    {
    public:
        physics_box_collision_shape_component();

        physics_box_collision_shape_component(
            const std::shared_ptr<physics::physics_box_collision>& box_collision) : m_box_collision(box_collision)
        {
        }

        ~physics_box_collision_shape_component() override = default;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<physics::physics_box_collision>& get_box_collision() const { return m_box_collision; }

        /* Setters */
        void set_box_collision(const std::shared_ptr<physics::physics_box_collision>& box_collision)
        {
            m_box_collision = box_collision;
        }

    private:
        std::shared_ptr<physics::physics_box_collision> m_box_collision;
    };
}
