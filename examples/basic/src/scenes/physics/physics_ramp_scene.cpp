#include "physics_ramp_scene.h"

#include <random>

#include "imgui.h"
#include "lifetime_component.h"
#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "physics/physics_material.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "physics/collision_shapes/physics_box_collision.h"
#include "physics/collision_shapes/physics_sphere_collision.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/models/model_factory.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_sphere_collision_shape_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scenes/scene_utils.h"

physics_ramp_scene::physics_ramp_scene() : scene("Physics Ramp Scene")
{
    initialize();
}

physics_ramp_scene::~physics_ramp_scene()
{
}

void physics_ramp_scene::on_update(float delta_time)
{
    scene::on_update(delta_time);

    if (!m_spawn_balls) return;

    m_balls_start_pos = {-8, 10, 7};
    m_balls_elapsed_time += delta_time;
    if (m_balls_elapsed_time >= m_balls_spawn_interval)
    {
        for (int i = 0; i < 6; i++)
        {
            create_ball(m_balls_start_pos, 0.35f);
            m_balls_start_pos.z -= 1.f;
        }
        m_balls_elapsed_time = 0.0f;
    }
}

void physics_ramp_scene::on_img_gui()
{
    if (ImGui::Begin("Scene Debug"))
    {
        ImGui::Checkbox("Spawn Balls", &m_spawn_balls);
    }
    ImGui::End();
}

void physics_ramp_scene::initialize()
{
    m_balls_spawn_interval = 1.0f;
    m_balls_elapsed_time = 0.0f;
    m_spawn_balls = false;

    scene_utils::create_environment(this);
    scene_utils::create_floor(this);

    create_ramp();
    create_wall();
}

void physics_ramp_scene::create_ramp()
{
    glm::vec3 ramp_size = {15, 1, 20};
    const std::shared_ptr<luly::scene::scene_actor>& floor_actor = create_actor("Ramp");
    // 1. Load up model.
    const std::shared_ptr<luly::renderer::model> model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/cube.obj");
    const std::shared_ptr<luly::assets::asset>& model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "floor-model", luly::assets::asset_type::model, model);
    luly::scene::model_renderer_component& model_renderer_component = floor_actor->add_component<
        luly::scene::model_renderer_component>(
        model_asset->get_data<luly::renderer::model>());
    // 2. Setup physics.
    const std::shared_ptr<luly::physics::physics_material>& physics_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);

    const luly::scene::transform_component& transform_component = floor_actor->get_component<
        luly::scene::transform_component>();

    transform_component.get_transform()->set_location({-5, 5, 0});
    // transform_component.get_transform()->set_rotation({0.5, 0, 0, -0.8});
    transform_component.get_transform()->set_scale(ramp_size);

    glm::vec3 half_extends = {ramp_size.x / 2, ramp_size.y / 2, ramp_size.z / 2};

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& floor_physics_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());

    const std::shared_ptr<luly::physics::physics_box_collision>& floor_collision_shape = std::make_shared<
        luly::physics::physics_box_collision>(floor_physics_actor, physics_material, half_extends);

    floor_physics_actor->add_collision_shape(floor_collision_shape);
    floor_physics_actor->set_kinematic(true);
    floor_physics_actor->initialize(this);

    floor_actor->add_component<luly::scene::physics_dynamic_actor_component>(floor_physics_actor);
    floor_actor->add_component<luly::scene::physics_material_component>(physics_material);
    floor_actor->add_component<luly::scene::physics_box_collision_shape_component>(floor_collision_shape);

    // 3. Setup material
    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo({1.0f, 1.0f, 1.0f, 1.0f}).
                                                                            build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    floor_actor->add_component<luly::scene::material_component>(material);
}

void physics_ramp_scene::create_wall()
{
    glm::vec3 wall_size = {5, 1, 20};
    const std::shared_ptr<luly::scene::scene_actor>& floor_actor = create_actor("Wall");
    // 1. Load up model.
    const std::shared_ptr<luly::renderer::model> model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/cube.obj");
    const std::shared_ptr<luly::assets::asset>& model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "wall-model", luly::assets::asset_type::model, model);
    floor_actor->add_component<
        luly::scene::model_renderer_component>(
        model_asset->get_data<luly::renderer::model>());
    // 2. Setup physics.
    const std::shared_ptr<luly::physics::physics_material>& physics_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);

    const luly::scene::transform_component& transform_component = floor_actor->get_component<
        luly::scene::transform_component>();

    transform_component.get_transform()->set_location({-5, 5, 0});
    transform_component.get_transform()->set_scale(wall_size);

    glm::vec3 half_extents = {wall_size.x / 2, wall_size.y / 2, wall_size.z / 2};

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& floor_physics_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());

    const std::shared_ptr<luly::physics::physics_box_collision>& floor_collision_shape = std::make_shared<
        luly::physics::physics_box_collision>(floor_physics_actor, physics_material, half_extents);

    floor_physics_actor->add_collision_shape(floor_collision_shape);
    floor_physics_actor->set_kinematic(true);
    floor_physics_actor->initialize(this);

    floor_actor->add_component<luly::scene::physics_dynamic_actor_component>(floor_physics_actor);
    floor_actor->add_component<luly::scene::physics_material_component>(physics_material);
    floor_actor->add_component<luly::scene::physics_box_collision_shape_component>(floor_collision_shape);

    // 3. Setup material
    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo({1.0f, 1.0f, 1.0f, 1.0f}).
                                                                            build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    floor_actor->add_component<luly::scene::material_component>(material);
}

void physics_ramp_scene::create_ball(const glm::vec3& position, float radius)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    radius = std::max(0.1f, dis(gen));

    const std::shared_ptr<luly::scene::scene_actor>& ball_actor = create_actor("Ball");
    // 1. Load up model.
    std::shared_ptr<luly::renderer::model> ball_model;
    if (!luly::assets::assets_manager::get().asset_already_registered("ball-model"))
    {
        const std::shared_ptr<luly::renderer::mesh>& mesh = luly::renderer::mesh_factory::create_sphere_mesh();
        const std::shared_ptr<luly::renderer::model> model =
            luly::renderer::model_factory::create_model_from_meshes({
                mesh
            });
        const auto& model_asset = luly::assets::asset_factory::create_asset<
            luly::renderer::model>(
            "ball-model", luly::assets::asset_type::model, model);
        ball_model = model_asset->get_data<luly::renderer::model>();
    }
    else
    {
        ball_model = luly::assets::assets_manager::get().get_asset<
            luly::assets::asset, luly::assets::asset_type::model>("ball-model")->get_data<luly::renderer::model>();
    }
    ball_actor->add_component<luly::scene::model_renderer_component>(ball_model);
    // 2. Set up transform.
    const luly::scene::transform_component& transform_component = ball_actor->get_component<
        luly::scene::transform_component>();

    transform_component.get_transform()->set_location(position);
    transform_component.get_transform()->set_scale(glm::vec3(radius));
    // 3. Setup physics.
    const std::shared_ptr<luly::physics::physics_material>& physics_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& ball_physics_dynamic_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());

    const std::shared_ptr<luly::physics::physics_sphere_collision>& sphere_collision_shape = std::make_shared<
        luly::physics::physics_sphere_collision>(ball_physics_dynamic_actor, physics_material, radius);

    ball_physics_dynamic_actor->add_collision_shape(sphere_collision_shape);
    ball_physics_dynamic_actor->initialize(this);

    ball_actor->add_component<luly::scene::physics_dynamic_actor_component>(ball_physics_dynamic_actor);
    ball_actor->add_component<luly::scene::physics_material_component>(physics_material);
    ball_actor->add_component<luly::scene::physics_sphere_collision_shape_component>(sphere_collision_shape);

    // 3. Setup material


    const glm::vec4 albedo = glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo(albedo).build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    ball_actor->add_component<luly::scene::material_component>(material);
    ball_actor->add_component<lifetime_component>(20.0f);
}
