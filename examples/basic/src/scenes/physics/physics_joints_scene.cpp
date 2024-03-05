#include "physics_joints_scene.h"

#include <random>

#include "assets/assets_manager.h"
#include "assets/asset_factory.h"
#include "physics/physics_utils.h"
#include "physics/actors/physics_actor.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "physics/collision_shapes/physics_box_collision.h"
#include "physics/constraints/physics_d6_joint.h"
#include "physics/constraints/physics_fixed_joint.h"
#include "physics/constraints/physics_spherical_joint.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/models/model_factory.h"
#include "scene/scene_manager.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
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

    // create_fixed_joint_example();
    create_chain();
}

void physics_joints_scene::create_fixed_joint_example()
{
    glm::vec3 cube_a_pos = {2, 6, 2};
    const std::shared_ptr<luly::scene::scene_actor>& cube_a = scene_utils::create_cube(
        this, glm::vec3(1.0f), cube_a_pos);

    glm::vec3 cube_b_pos = {4, 6, 2};
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

    physx::PxTransform localFrame0(physx::PxVec3(4.0f, 10.0f, 4.0f));
    physx::PxTransform localFrame1(physx::PxVec3(-2.0f, 0.0f, 0.0f));

    const std::shared_ptr<luly::physics::physics_spherical_joint>& physics_spherical_joint = std::make_shared<
        luly::physics::physics_spherical_joint>(nullptr, localFrame0,
                                                cube_b_dynamic_actor,
                                                localFrame1);
    physics_spherical_joint->set_limit_cone(glm::pi<float>() / 2, glm::pi<float>() / 6);
    physics_spherical_joint->set_spherical_joint_flag(physx::PxSphericalJointFlag::eLIMIT_ENABLED, true);
    /*
    const std::shared_ptr<luly::physics::physics_fixed_joint>& physics_fixed_joint = std::make_shared<
        luly::physics::physics_fixed_joint>(cube_a_dynamic_actor, physx::PxTransform(physx::PxIdentity),
                                            cube_b_dynamic_actor,
                                            physx::PxTransform(physx::PxIdentity));*/
}

void physics_joints_scene::create_chain()
{
    float separation = 2.5f;
    int length = 5;
    glm::vec3 start_location = {0, 4, 0};
    glm::vec3 link_scale = {2.0f, 0.5f, 0.5f};

    std::shared_ptr<luly::physics::physics_dynamic_actor> previous_actor = nullptr;
    physx::PxVec3 offset(separation / 2, 0, 0);
    physx::PxTransform start_position(luly::physics::physics_utils::convert_glm_vec3_to_physx(start_location));
    physx::PxTransform local_transform = physx::PxTransform(offset);

    const std::shared_ptr<luly::physics::physics_material>& phys_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);

    std::shared_ptr<luly::renderer::model> cube_model;
    if (!luly::assets::assets_manager::get().asset_already_registered("cube-model"))
    {
        const std::shared_ptr<luly::renderer::model> model =
            luly::renderer::model_factory::create_model_from_file("assets/models/cube.obj");
        const auto& model_asset = luly::assets::asset_factory::create_asset<
            luly::renderer::model>(
            "cube-model", luly::assets::asset_type::model, model);
        cube_model = model_asset->get_data<luly::renderer::model>();
    }
    else
    {
        cube_model = luly::assets::assets_manager::get().get_asset<
            luly::assets::asset, luly::assets::asset_type::model>("cube-model")->get_data<luly::renderer::model>();
    }

    for (int i = 0; i < length; ++i)
    {
        /* Box collision shape */
        const std::shared_ptr<luly::physics::physics_box_collision>& box_collision_shape = std::make_shared<
            luly::physics::physics_box_collision>(
            phys_material, link_scale);

        /* Setup physics dynamic actor */
        physx::PxTransform curr_trans = start_position * local_transform;
        const std::shared_ptr<luly::physics::physics_dynamic_actor>& dynamic_actor = std::make_shared<
            luly::physics::physics_dynamic_actor>(
            luly::physics::physics_utils::convert_physx_vec3_to_glm(curr_trans.p),
            luly::physics::physics_utils::convert_physx_quat_to_glm(curr_trans.q));
        dynamic_actor->add_collision_shape(box_collision_shape);
        dynamic_actor->initialize(this);

        /* Create the scene actor */
        const std::shared_ptr<luly::scene::scene_actor>& scene_actor = create_actor("chain actor");

        /* Setup transform */
        glm::vec3 location = luly::physics::physics_utils::convert_physx_vec3_to_glm(
            dynamic_actor->get_physx_rigid_dynamic_actor()->getGlobalPose().p);
        glm::quat rotation = luly::physics::physics_utils::convert_physx_quat_to_glm(
            dynamic_actor->get_physx_rigid_dynamic_actor()->getGlobalPose().q);
        scene_actor->get_component<luly::scene::transform_component>().get_transform()->set_location(location);
        scene_actor->get_component<luly::scene::transform_component>().get_transform()->set_rotation(rotation);
        scene_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale(link_scale);

        /* Setup model renderer */
        scene_actor->add_component<luly::scene::model_renderer_component>(cube_model);

        /* Setup material renderer */
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        const glm::vec4 albedo = glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);

        const luly::renderer::material_specification& material_specification =
            std::make_shared<luly::renderer::material_specification_builder>()->with_albedo(albedo).build();
        const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
            luly::renderer::material>(
            material_specification);
        scene_actor->add_component<luly::scene::material_component>(material);

        /* Setup physics components */
        scene_actor->add_component<luly::scene::physics_dynamic_actor_component>(dynamic_actor);
        scene_actor->add_component<luly::scene::physics_material_component>(phys_material);
        scene_actor->add_component<luly::scene::physics_box_collision_shape_component>(box_collision_shape);

        /* Joint creation */
        const std::shared_ptr<luly::physics::physics_d6_joint>& joint = std::make_shared<
            luly::physics::physics_d6_joint>(previous_actor,
                                             previous_actor ? physx::PxTransform(offset) : start_position,
                                             dynamic_actor, physx::PxTransform(-offset));

        joint->set_motion(physx::PxD6Axis::eSWING1, physx::PxD6Motion::eFREE);
        joint->set_motion(physx::PxD6Axis::eSWING2, physx::PxD6Motion::eFREE);
        joint->set_motion(physx::PxD6Axis::eTWIST, physx::PxD6Motion::eFREE);
        joint->set_drive(physx::PxD6Drive::eSLERP, physx::PxD6JointDrive(10.0f, 300.0f, 100.0f, true));

        /* Setup joint component */
        //   scene_actor->add_component<scene::physics_d6_joint_component>(joint);

        // Update the position for the next actor
        previous_actor = dynamic_actor;
        local_transform.p.x += separation;
    }
}
