#pragma once

#include "physics/constraints/physics_d6_joint.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class physics_d6_joint_component : public base_component
    {
    public:
        physics_d6_joint_component(const std::shared_ptr<physics::physics_d6_joint>& physics_d6_joint);
        ~physics_d6_joint_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<physics::physics_d6_joint>& get_physics_d6_joint() const { return m_physics_d6_joint; }

        /* Setters */
        void set_physics_d6_joint(const std::shared_ptr<physics::physics_d6_joint>& physics_d6_joint) { m_physics_d6_joint = physics_d6_joint; }

    private:
        std::shared_ptr<physics::physics_d6_joint> m_physics_d6_joint;
    };
}
