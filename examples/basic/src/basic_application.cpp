#include "basic_application.h"

#include <random>
#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "scene/scene.h"
#include "events/event_dispatcher.h"
#include "lifetime_component.h"
#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "physics/physics_utils.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_sphere_collision_shape_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scenes/pbr_mask_scene.h"
#include "scenes/physics/physics_joints_scene.h"
#include "scenes/physics/physics_pyramid_scene.h"
#include "scenes/physics/physics_ramp_scene.h"
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

    current_scene->on_img_gui();

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
    const std::shared_ptr<physics_pyramid_scene>& physics_pyramid_scene = std::make_shared<class
        physics_pyramid_scene>();
    luly::scene::scene_manager::get().add_scene(physics_pyramid_scene);

    const std::shared_ptr<pbr_mask_scene>& pbr_mask_scene = std::make_shared<class pbr_mask_scene>();
    luly::scene::scene_manager::get().add_scene(pbr_mask_scene);

    const std::shared_ptr<physics_joints_scene>& physics_joints_scene = std::make_shared<class
        physics_joints_scene>();
    luly::scene::scene_manager::get().add_scene(physics_joints_scene);

    const std::shared_ptr<physics_ramp_scene>& physics_ramp_scene = std::make_shared<class
        physics_ramp_scene>();
    luly::scene::scene_manager::get().add_scene(physics_ramp_scene);

    luly::scene::scene_manager::get().switch_scene(physics_pyramid_scene);
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

    const std::shared_ptr<luly::physics::physics_dynamic_actor>& ball_physics_dynamic_actor = std::make_shared<
        luly::physics::physics_dynamic_actor>(transform_component.get_transform()->get_location(),
                                              transform_component.get_transform()->get_rotation());

    const std::shared_ptr<luly::physics::physics_sphere_collision>& sphere_collision_shape = std::make_shared<
        luly::physics::physics_sphere_collision>(ball_physics_dynamic_actor, physics_material, radius);


    const glm::vec3& impulse_force = scene->get_camera_manager()->get_perspective_camera()->get_front() * impulse;
    ball_physics_dynamic_actor->set_linear_velocity(impulse_force);
    ball_physics_dynamic_actor->add_collision_shape(sphere_collision_shape);
    ball_physics_dynamic_actor->set_mass(5.0f);
    ball_physics_dynamic_actor->initialize(scene.get());

    ball_actor->add_component<luly::scene::physics_dynamic_actor_component>(ball_physics_dynamic_actor);
    ball_actor->add_component<luly::scene::physics_material_component>(physics_material);
    ball_actor->add_component<luly::scene::physics_sphere_collision_shape_component>(sphere_collision_shape);

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
    ball_actor->add_component<luly::scene::material_component>(material);

    // 4. Add lifetime
    ball_actor->add_component<lifetime_component>(5.0f);
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
