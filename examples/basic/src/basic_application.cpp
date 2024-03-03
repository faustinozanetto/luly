#include "basic_application.h"

#include <random>
#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "renderer/textures/texture_factory.h"
#include "scene/scene.h"

#include <events/event_dispatcher.h>

#include "lifetime_component.h"
#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "physics/physics_material.h"
#include "physics/physics_utils.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "physics/actors/physics_static_actor.h"
#include "physics/collision_shapes/physics_box_collision.h"
#include "physics/collision_shapes/physics_sphere_collision.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_sphere_collision_shape_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/rendering/skybox_component.h"
#include "time/app_time.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    m_engine_ui = std::make_shared<luly::ui::engine_ui>();
    setup_scene();

    m_engine_ui->set_render_target(
        luly::renderer::scene_renderer::get_render_pass<luly::renderer::final_pass>(
            luly::renderer::render_pass_type::final_pass)->get_output("final_output").output);
}

basic_application::~basic_application()
{
}

void basic_application::on_create()
{
}

void basic_application::on_update()
{
    application::on_update();

    const std::shared_ptr<luly::scene::scene>& current_scene = luly::scene::scene_manager::get().get_current_scene();
    if (!current_scene) return;

    m_engine_ui->begin_frame();

    const auto& view = current_scene->get_registry()->view<lifetime_component>();
    for (const auto& [actor, lifetime_component] : view.each())
    {
        lifetime_component.on_update(luly::app_time::get_delta_time());
    }

    const std::shared_ptr<luly::renderer::perspective_camera>& camera = current_scene->get_camera_manager()->
        get_perspective_camera();

    luly::renderer::scene_renderer::begin_render(camera);
    luly::renderer::scene_renderer::end_render();

    m_engine_ui->on_update();
    m_engine_ui->end_frame();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
    luly::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<luly::events::key_pressed_event>(BIND_EVENT_FN(on_key_pressed_event));

    m_engine_ui->on_event(event);
}

bool basic_application::on_key_pressed_event(const luly::events::key_pressed_event& key_pressed_event)
{
    const std::shared_ptr<luly::scene::scene>& current_scene = luly::scene::scene_manager::get().get_current_scene();
    if (key_pressed_event.get_key_code() == luly::input::key::space)
    {
        create_ball(current_scene, 0.25f, 75);
        return true;
    }
    return false;
}

void basic_application::setup_scene()
{
    const std::shared_ptr<luly::scene::scene>& scene = std::make_shared<luly::scene::scene>("Test Scene");

    luly::scene::scene_manager::get().add_scene(scene);

    scene->get_camera_manager()->get_perspective_camera()->set_far_clip(250.0f);
    scene->get_camera_manager()->get_perspective_camera()->set_position({-5.0f, 5.0f, -17.f});
    scene->get_camera_manager()->get_perspective_camera()->set_pitch(-4.5f);
    scene->get_camera_manager()->get_perspective_camera()->set_yaw(-300.5f);

    const std::shared_ptr<luly::scene::scene_actor>& dir_light_actor = scene->create_actor("Sun Light");
    const std::shared_ptr<luly::renderer::directional_light>& directional_light = std::make_shared<
        luly::renderer::directional_light>();
    directional_light->set_direction(86.0f, 8.0f);
    dir_light_actor->add_component<luly::scene::directional_light_component>(directional_light);

    // Skybox
    const std::shared_ptr<luly::scene::scene_actor>& skybox_actor = scene->create_actor("Skybox");
    const std::shared_ptr<luly::renderer::texture_2d>& environment_texture =
        luly::renderer::texture_factory::create_environment_texture_from_file(
            "assets/hdris/blue_photo_studio_4k.hdr");
    luly::scene::skybox_component& skybox_component = skybox_actor->add_component<luly::scene::skybox_component>(
        environment_texture
    );
    skybox_component.set_intensity(0.5f);

    // Create emissive cube
    /*
    const std::shared_ptr<luly::scene::scene_actor>& emissive_cube_actor = scene->create_actor("Emissive Cube");
    emissive_cube_actor->add_component<luly::scene::model_renderer_component>(
        luly::renderer::model_factory::create_model_from_meshes({
            luly::renderer::mesh_factory::create_cube_mesh()
        }));

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_emissive({0.25f, 0.35f, 0.65f}).
                                                                            with_emissive_strength(5.0f).build();
    const std::shared_ptr<luly::renderer::material>& emissive_material = std::make_shared<luly::renderer::material>(
        material_specification);
    emissive_cube_actor->add_component<luly::scene::material_component>(emissive_material);
*/
    /*
        const std::shared_ptr<luly::scene::scene_actor>& sponza_actor = scene->create_actor("Bistro");
        sponza_actor->add_component<luly::scene::model_renderer_component>(
            luly::renderer::model_factory::create_model_from_file("assets/models/Bistro/bistro.gltf"));
        //sponza_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({0.01f,0.01f,0.01f});
    
        // Create emissive cube
        /*
        const std::shared_ptr<luly::scene::scene_actor>& emissive_cube_actor = scene->create_actor("Emissive Cube");
        emissive_cube_actor->add_component<luly::scene::model_renderer_component>(
            luly::renderer::model_factory::create_model_from_meshes({
                luly::renderer::mesh_factory::create_cube_mesh()
            }));
    
        const luly::renderer::material_specification& material_specification =
            std::make_shared<luly::renderer::material_specification_builder>()->with_emissive({0.25f, 0.35f, 0.65f}).
                                                                                with_emissive_strength(5.0f).build();
        const std::shared_ptr<luly::renderer::material>& emissive_material = std::make_shared<luly::renderer::material>(
            material_specification);
        emissive_cube_actor->add_component<luly::scene::material_component>(emissive_material);
    */
    /*
        const std::shared_ptr<luly::renderer::mesh>& sphere_mesh = luly::renderer::mesh_factory::create_sphere_mesh(
            10, 10, 0.05f);
        float grid_gap = 0.2f;
        int rows = 1;
        int cols = 1;
    
        float startX = -(rows - 1) * grid_gap * 0.5f;
        float startY = -(cols - 1) * grid_gap * 0.5f;
    
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                float xPos = startX + row * grid_gap;
                float yPos = startY + col * grid_gap;
    
                // Calculate color based on row and col
                glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
                color.r += row / static_cast<float>(rows);
                color.g += col / static_cast<float>(cols);
    
                const std::shared_ptr<luly::scene::scene_actor>& point_light_actor = scene->create_actor(
                    std::format("Point Light: [{},{}]", row, col));
    
                // Point light component with dynamic color
                point_light_actor->add_component<luly::scene::point_light_component>(
                    std::make_shared<luly::renderer::point_light>(color));
    
                // Model renderer component
                point_light_actor->add_component<luly::scene::model_renderer_component>(
                    luly::renderer::model_factory::create_model_from_meshes({
                        sphere_mesh
                    }));
    
                // Transform component.
                const luly::scene::transform_component& transform_component =
                    point_light_actor->get_component<luly::scene::transform_component>();
    
                transform_component.get_transform()->set_location(
                    glm::vec3(xPos, yPos, 0.75f)
                                                                            );
                transform_component.get_transform()->set_scale({
                    0.3f, 0.3f, 0.3f
                });
            }
        }*/


    create_floor(scene);
    create_cubes_pyramid(scene);

    luly::scene::scene_manager::get().switch_scene(scene);
}

void basic_application::create_mask(const std::shared_ptr<luly::scene::scene>& scene)
{
    const std::shared_ptr<luly::scene::scene_actor>& actor = scene->create_actor("Plague Mask");
    const luly::scene::transform_component& actor_transform = actor->get_component<luly::scene::transform_component>();
    actor_transform.get_transform()->set_location({-0.15f, 1.1f, -0.5f});
    actor_transform.get_transform()->set_scale({0.85f, 0.85f, 0.85f});

    const std::shared_ptr<luly::renderer::model> plague_mask_model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/PlagueMask/PlagueMask.obj");
    const std::shared_ptr<luly::assets::asset>& plague_mask_model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "plague-mask-model", luly::assets::asset_type::model, plague_mask_model);
    actor->add_component<luly::scene::model_renderer_component>(
        plague_mask_model_asset->get_data<luly::renderer::model>());

    const std::shared_ptr<luly::renderer::texture_2d>& albedo_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Base_Color.png");
    const std::shared_ptr<luly::assets::asset>& albedo_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-albedo-texture", luly::assets::asset_type::texture, albedo_texture);
    luly::renderer::material_texture albedo = {albedo_texture, luly::renderer::material_texture_type::albedo};

    const std::shared_ptr<luly::renderer::texture_2d>& normal_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Normal_DirectX.png");
    const std::shared_ptr<luly::assets::asset>& normal_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-normal-texture", luly::assets::asset_type::texture, normal_texture);
    luly::renderer::material_texture normal = {normal_texture, luly::renderer::material_texture_type::normal};

    const std::shared_ptr<luly::renderer::texture_2d>& metallic_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Metallic.png");
    const std::shared_ptr<luly::assets::asset>& metallic_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-metallic-texture", luly::assets::asset_type::texture, metallic_texture);
    luly::renderer::material_texture metallic = {
        metallic_texture, luly::renderer::material_texture_type::metallic
    };

    const std::shared_ptr<luly::renderer::texture_2d>& roughness_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Roughness.png");
    const std::shared_ptr<luly::assets::asset>& roughness_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-roughness-texture", luly::assets::asset_type::texture, roughness_texture);
    luly::renderer::material_texture roughness = {
        roughness_texture, luly::renderer::material_texture_type::roughness
    };

    const std::shared_ptr<luly::renderer::texture_2d>& ao_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Mixed_AO.png");
    const std::shared_ptr<luly::assets::asset>& ao_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-ao-texture", luly::assets::asset_type::texture, ao_texture);
    luly::renderer::material_texture ao = {
        ao_texture, luly::renderer::material_texture_type::ambient_occlusion
    };

    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});
    textures.insert({luly::renderer::material_texture_type::ambient_occlusion, ao});

    const luly::renderer::material_specification& helmet_material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->
        with_textures(textures).build();

    const std::shared_ptr<luly::renderer::material>& plague_mask_material = std::make_shared<
        luly::renderer::material>(
        helmet_material_specification);
    plague_mask_material->set_texture_channel_mode(luly::renderer::material_texture_type::roughness,
                                                   luly::renderer::material_texture_channel_mode::green);
    plague_mask_material->set_texture_channel_mode(luly::renderer::material_texture_type::metallic,
                                                   luly::renderer::material_texture_channel_mode::blue);

    const std::shared_ptr<luly::assets::asset>& tv_material_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::material>(
        "plague-mask-material", luly::assets::asset_type::material, plague_mask_material);

    actor->add_component<luly::scene::material_component>(tv_material_asset->get_data<luly::renderer::material>());
}

void basic_application::create_floor(const std::shared_ptr<luly::scene::scene>& scene)
{
    glm::vec3 floor_size = {25.0f, 1.0f, 25.0f};
    const std::shared_ptr<luly::scene::scene_actor>& floor_actor = scene->create_actor("Floor");
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

    const std::shared_ptr<luly::physics::physics_box_collision>& floor_collision_shape = std::make_shared<
        luly::physics::physics_box_collision>(physics_material, floor_size);

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
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo({0.25f, 0.25f, 0.25f}).build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    floor_actor->add_component<luly::scene::material_component>(material);
}

void basic_application::create_ball(const std::shared_ptr<luly::scene::scene>& scene, float radius, float impulse)
{
    const std::shared_ptr<luly::scene::scene_actor>& ball_actor = scene->create_actor("Ball");
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
    const glm::vec3 location = scene->get_camera_manager()->get_perspective_camera()->get_position();

    transform_component.get_transform()->set_location(location);
    transform_component.get_transform()->set_scale(glm::vec3(radius));
    // 3. Setup physics.
    const std::shared_ptr<luly::physics::physics_material>& physics_material = std::make_shared<
        luly::physics::physics_material>(0.5f, 0.5f, 0.6f);
    const std::shared_ptr<luly::physics::physics_sphere_collision>& sphere_collision_shape = std::make_shared<
        luly::physics::physics_sphere_collision>(physics_material, radius);

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& ball_physics_dynamic_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());
    const glm::vec3& impulse_force = scene->get_camera_manager()->get_perspective_camera()->get_front() * impulse;
    ball_physics_dynamic_actor->set_linear_velocity(impulse_force);
    ball_physics_dynamic_actor->add_collision_shape(sphere_collision_shape);
    ball_physics_dynamic_actor->initialize();

    ball_actor->add_component<luly::scene::physics_dynamic_actor_component>(ball_physics_dynamic_actor);
    ball_actor->add_component<luly::scene::physics_material_component>(physics_material);
    ball_actor->add_component<luly::scene::physics_sphere_collision_shape_component>(sphere_collision_shape);

    // 3. Setup material
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    const glm::vec3 albedo = glm::vec3(dis(gen), dis(gen), dis(gen));

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo(albedo).build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    ball_actor->add_component<luly::scene::material_component>(material);

    // 4. Add lifetime
    ball_actor->add_component<lifetime_component>(5.0f);
}

void basic_application::create_cube(const std::shared_ptr<luly::scene::scene>& scene, glm::vec3 size, glm::vec3 pos)
{
    const std::shared_ptr<luly::scene::scene_actor>& cube_actor = scene->create_actor("Physics Cube");
    // 1. Load up model.
    std::shared_ptr<luly::renderer::model> cube_model;
    if (!luly::assets::assets_manager::get().asset_already_registered("cube-model"))
    {
        const std::shared_ptr<luly::renderer::mesh>& mesh = luly::renderer::mesh_factory::create_cube_mesh();
        const std::shared_ptr<luly::renderer::model> model =
            luly::renderer::model_factory::create_model_from_meshes({
                mesh
            });
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

    const glm::vec3 albedo = glm::vec3(dis(gen), dis(gen), dis(gen));

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_albedo(albedo).build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(
        material_specification);
    cube_actor->add_component<luly::scene::material_component>(material);
}

void basic_application::create_cubes_pyramid(const std::shared_ptr<luly::scene::scene>& scene)
{
    int levels = 10;

    glm::vec3 start_pos = {0, 4, 0};

    for (int i = 0; i < levels; i++)
    {
        int cubes_count = levels - i;
        float half_cube_width = 0.5f; // Half of the cube width
        for (int j = 0; j < cubes_count; j++)
        {
            // Adjust cube position based on current level and cube index
            glm::vec3 cube_pos = {start_pos.x + j - (cubes_count - 1) * 0.5f + half_cube_width, start_pos.y + i, 0};
            create_cube(scene, glm::vec3(1.0f), cube_pos);
        }
    }
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
