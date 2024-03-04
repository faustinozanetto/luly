#include "physics_joints_scene.h"

#include "physics/physics_utils.h"
#include "physics/actors/physics_actor.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "physics/constraints/physics_fixed_joint.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "scenes/scene_utils.h"

physics_joints_scene::physics_joints_scene() : scene("Physics Joints Scene")
{
    initialize();
}

physics_joints_scene::~physics_joints_scene()
{
}

void physics_joints_scene::initialize()
{
    scene_utils::create_environment(this);
    scene_utils::create_floor(this);

    create_fixed_joint_example();
}

void physics_joints_scene::create_fixed_joint_example()
{
    glm::vec3 cube_a_pos = {2, 2, 2};
    const std::shared_ptr<luly::scene::scene_actor>& cube_a = scene_utils::create_cube(
        this, glm::vec3(1.0f), cube_a_pos);

    glm::vec3 cube_b_pos = {4, 4, 4};
    const std::shared_ptr<luly::scene::scene_actor>& cube_b = scene_utils::create_cube(
        this, glm::vec3(1.0f), cube_b_pos);

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& cube_a_dynamic_actor = cube_a->get_component<
        luly::scene::physics_dynamic_actor_component>().get_dynamic_actor();
    const std::shared_ptr<luly::math::transform>& cube_a_transform = cube_a->get_component<
        luly::scene::transform_component>().get_transform();
    const physx::PxTransform& physx_cube_a_transform = luly::physics::physics_utils::create_transform_from_glm(
        cube_a_transform->get_location(), cube_a_transform->get_rotation());

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& cube_b_dynamic_actor = cube_b->get_component<
        luly::scene::physics_dynamic_actor_component>().get_dynamic_actor();
    const std::shared_ptr<luly::math::transform>& cube_b_transform = cube_b->get_component<
        luly::scene::transform_component>().get_transform();
    const physx::PxTransform& physx_cube_b_transform = luly::physics::physics_utils::create_transform_from_glm(
        cube_b_transform->get_location(), cube_b_transform->get_rotation());

    const std::shared_ptr<luly::physics::physics_fixed_joint>& physics_fixed_joint = std::make_shared<
        luly::physics::physics_fixed_joint>(cube_a_dynamic_actor, physx_cube_a_transform, cube_b_dynamic_actor,
                                            physx_cube_b_transform);
}
