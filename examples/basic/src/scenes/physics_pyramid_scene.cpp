#include "physics_pyramid_scene.h"

#include <random>

#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "physics/physics_material.h"
#include "physics/physics_utils.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "physics/actors/physics_static_actor.h"
#include "physics/collision_shapes/physics_box_collision.h"
#include "renderer/lights/directional_light.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/models/model_factory.h"
#include "renderer/textures/texture_factory.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/rendering/skybox_component.h"

physics_pyramid_scene::physics_pyramid_scene() : scene("Physics Pyramid Scene")
{
    initialize();
}

physics_pyramid_scene::~physics_pyramid_scene()
{
}

void physics_pyramid_scene::initialize()
{
    create_floor();
    create_cubes_pyramid();

    // Setup directional light
    const std::shared_ptr<luly::scene::scene_actor>& dir_light_actor = create_actor("Sun Light");
    const std::shared_ptr<luly::renderer::directional_light>& directional_light = std::make_shared<
        luly::renderer::directional_light>();
    directional_light->set_direction(86.0f, 8.0f);
    dir_light_actor->add_component<luly::scene::directional_light_component>(directional_light);

    // Setup Skybox
    const std::shared_ptr<luly::scene::scene_actor>& skybox_actor = create_actor("Skybox");
    const std::shared_ptr<luly::renderer::texture_2d>& environment_texture =
        luly::renderer::texture_factory::create_environment_texture_from_file(
            "assets/hdris/blue_photo_studio_4k.hdr");
    luly::scene::skybox_component& skybox_component = skybox_actor->add_component<luly::scene::skybox_component>(
        environment_texture
    );
    skybox_component.set_intensity(0.5f);
}


void physics_pyramid_scene::create_floor()
{
    glm::vec3 floor_size = {50.0f, 1.0f, 50.0f};
    const std::shared_ptr<luly::scene::scene_actor>& floor_actor = create_actor("Floor");
    // 1. Load up model.
    const std::shared_ptr<luly::renderer::model> floor_model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/cube.obj");
    const std::shared_ptr<luly::assets::asset>& floor_model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "floor-model", luly::assets::asset_type::model, floor_model);
    luly::scene::model_renderer_component& model_renderer_component = floor_actor->add_component<
        luly::scene::model_renderer_component>(
        floor_model_asset->get_data<luly::renderer::model>());
    model_renderer_component.set_casts_shadows(false);
    // 2. Setup physics.
    const std::shared_ptr<luly::physics::physics_material>& physics_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);

    const luly::scene::transform_component& transform_component = floor_actor->get_component<
        luly::scene::transform_component>();

    transform_component.get_transform()->set_location({0, 0, 0});
    transform_component.get_transform()->set_scale(floor_size);

    glm::vec3 half_extends = {floor_size.x / 2, floor_size.y / 2, floor_size.z / 2};

    const std::shared_ptr<luly::physics::physics_box_collision>& floor_collision_shape = std::make_shared<
        luly::physics::physics_box_collision>(physics_material, half_extends);

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& floor_physics_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());
    floor_physics_actor->set_kinematic(true);

    floor_physics_actor->add_collision_shape(floor_collision_shape);
    floor_physics_actor->initialize();

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

void physics_pyramid_scene::create_cube(glm::vec3 size, glm::vec3 pos)
{
    const std::shared_ptr<luly::scene::scene_actor>& cube_actor = create_actor("Physics Cube");
    // 1. Load up model.
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
    cube_actor->add_component<luly::scene::model_renderer_component>(cube_model);
    // 2. Set up transform.
    const luly::scene::transform_component& transform_component = cube_actor->get_component<
        luly::scene::transform_component>();

    transform_component.get_transform()->set_location(pos);
    transform_component.get_transform()->set_scale(size);
    // 3. Setup physics.
    const std::shared_ptr<luly::physics::physics_material>& physics_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);
    glm::vec3 half_extents = {size.x / 2, size.y / 2, size.z / 2};
    const std::shared_ptr<luly::physics::physics_box_collision>& box_collision_shape = std::make_shared<
        luly::physics::physics_box_collision>(physics_material, half_extents);

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& cube_physics_dynamic_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());
    cube_physics_dynamic_actor->set_mass(0.2f);
    cube_physics_dynamic_actor->add_collision_shape(box_collision_shape);
    cube_physics_dynamic_actor->initialize();

    cube_actor->add_component<luly::scene::physics_dynamic_actor_component>(cube_physics_dynamic_actor);
    cube_actor->add_component<luly::scene::physics_material_component>(physics_material);
    cube_actor->add_component<luly::scene::physics_box_collision_shape_component>(box_collision_shape);

    // 3. Setup material
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    const glm::vec4 albedo = glm::vec4(dis(gen), dis(gen), dis(gen), 1.0f);

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo(albedo).build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    cube_actor->add_component<luly::scene::material_component>(material);
}

void physics_pyramid_scene::create_cubes_pyramid()
{
    int levels = 8;
    glm::vec3 start_pos = {0, 4, 0};

    for (int i = 0; i < levels; i++)
    {
        int cubes_count = levels - i;
        float half_cube_width = 0.5f;
        for (int j = 0; j < cubes_count; j++)
        {
            const glm::vec3 cube_pos = {
                start_pos.x + j - (cubes_count - 1) * 0.5f + half_cube_width, start_pos.y + i, 0
            };
            create_cube(glm::vec3(1.0f), cube_pos);
        }
    }
}
