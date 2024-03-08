#include "lypch.h"
#include "physics_d6_joint_component.h"

namespace luly::scene
{
    physics_d6_joint_component::physics_d6_joint_component(
        const std::shared_ptr<physics::physics_d6_joint>& physics_d6_joint)
    {
        m_physics_d6_joint = physics_d6_joint;
    }

    physics_d6_joint_component::~physics_d6_joint_component()
    {
    }

    void physics_d6_joint_component::initialize()
    {
    }
}
