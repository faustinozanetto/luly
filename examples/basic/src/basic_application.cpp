#include "basic_application.h"

#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/textures/texture_factory.h"
#include "scene/scene.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/lights/spot_light_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/rendering/skybox_component.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    luly::ui::engine_ui::initialize();

    setup_scene();

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_DEPTH_TEST);

    luly::ui::engine_ui::set_render_target(
        luly::renderer::scene_renderer::get_data().final_pass->get_output("final_output").output);
}

basic_application::~basic_application()
{
    luly::ui::engine_ui::shutdown();
}

void basic_application::on_create()
{
}

void basic_application::on_update()
{
    application::on_update();

    const std::shared_ptr<luly::scene::scene>& current_scene = luly::scene::scene_manager::get().get_current_scene();
    if (!current_scene) return;

    luly::ui::engine_ui::begin_frame();

    const std::shared_ptr<luly::renderer::perspective_camera>& camera = current_scene->get_camera_manager()->
        get_perspective_camera();

    luly::renderer::scene_renderer::begin_render(camera);
    luly::renderer::scene_renderer::end_render();

    luly::ui::engine_ui::on_update();
    luly::ui::engine_ui::end_frame();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
    luly::events::event_dispatcher dispatcher(event);
    dispatcher.dispatch<luly::events::key_pressed_event>(BIND_EVENT_FN(basic_application::on_key_pressed_event));
}

bool basic_application::on_key_pressed_event(const luly::events::key_pressed_event& key_pressed_event)
{
    const std::shared_ptr<luly::scene::scene>& current_scene = luly::scene::scene_manager::get().get_current_scene();
    if (!current_scene) return false;

    const std::shared_ptr<luly::renderer::perspective_camera_controller>& camera_controller = current_scene->
        get_camera_manager()->get_perspective_camera_controller();

    if (key_pressed_event.get_key_code() == luly::input::key::w)
    {
        camera_controller->process_keyboard_input(
            luly::renderer::camera_keyboard_direction::forward, luly::app_time::get_delta_time());
        return true;
    }
    if (key_pressed_event.get_key_code() == luly::input::key::s)
    {
        camera_controller->process_keyboard_input(
            luly::renderer::camera_keyboard_direction::backward, luly::app_time::get_delta_time());
        return true;
    }
    if (key_pressed_event.get_key_code() == luly::input::key::a)
    {
        camera_controller->process_keyboard_input(
            luly::renderer::camera_keyboard_direction::left, luly::app_time::get_delta_time());
        return true;
    }
    if (key_pressed_event.get_key_code() == luly::input::key::d)
    {
        camera_controller->process_keyboard_input(
            luly::renderer::camera_keyboard_direction::right, luly::app_time::get_delta_time());
        return true;
    }
    if (key_pressed_event.get_key_code() == luly::input::key::e)
    {
        camera_controller->process_keyboard_input(
            luly::renderer::camera_keyboard_direction::down, luly::app_time::get_delta_time());
        return true;
    }
    if (key_pressed_event.get_key_code() == luly::input::key::q)
    {
        camera_controller->process_keyboard_input(
            luly::renderer::camera_keyboard_direction::up, luly::app_time::get_delta_time());
        return true;
    }

    return false;
}

void basic_application::setup_scene()
{
    const std::shared_ptr<luly::scene::scene>& scene = std::make_shared<luly::scene::scene>("Test Scene");

    luly::scene::scene_manager::get().add_scene(scene);
    luly::scene::scene_manager::get().switch_scene(scene);

    /*
    const auto& dir_light_actor = scene->create_actor("Light Emitter");
    dir_light_actor->add_component<luly::scene::directional_light_component>(
        std::make_shared<luly::renderer::directional_light>(glm::vec3(0.85f), glm::vec3(0.8f, 0.3f, -5.0f))); /*
    */

    auto sphere_mesh = luly::renderer::mesh_factory::create_sphere_mesh(16, 16, 0.1f);
    for (int i = 0; i < 3; i++)
    {
        const auto& point_light_actor = scene->create_actor("Point Light Emitter: " + std::to_string(i));
        point_light_actor->add_component<luly::scene::point_light_component>(
            std::make_shared<luly::renderer::point_light>(glm::vec3(0.85f), glm::vec3(0.0f)));
        point_light_actor->add_component<luly::scene::model_renderer_component>(
            luly::renderer::model_factory::create_model_from_meshes({
                sphere_mesh
            }));
        point_light_actor->get_component<luly::scene::transform_component>().get_transform()->set_location({
            -1 + i / 2, 0, 0.5f
        });
        point_light_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({
            0.3f, 0.3f, 0.3f
        });
    }

    const std::shared_ptr<luly::scene::scene_actor>& skybox_actor = scene->create_actor("Skybox Actor");
    skybox_actor->add_component<luly::scene::skybox_component>(
        luly::renderer::texture_factory::create_environment_texture_from_file(
            "assets/hdris/kart_club_4k.hdr"));

    /*
    const auto& spot_light_actor = scene->create_actor("Spot Light Emitter");
    spot_light_actor->add_component<luly::scene::spot_light_component>(
        std::make_shared<luly::renderer::spot_light>(glm::vec3(0.85f), glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), 12.5f,
                                                     25.0f));
    spot_light_actor->get_component<luly::scene::transform_component>().get_transform()->set_location({0, 0, 2});
*/
    m_actor = scene->create_actor("Test Model");
    m_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({1.5f, 1.5f, 1.5f});

    // Loading TV model and creating asset.
    const std::shared_ptr<luly::renderer::model> tv_model = luly::renderer::model_factory::create_model_from_file(
        "assets/models/tv/tv.obj");
    const std::shared_ptr<luly::assets::asset>& model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "tv-model", luly::assets::asset_type::model, tv_model);
    m_actor->add_component<luly::scene::model_renderer_component>(model_asset->get_data<luly::renderer::model>());

    const std::shared_ptr<luly::renderer::texture_2d>& albedo_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-albedo.png");
    const std::shared_ptr<luly::assets::asset>& albedo_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-albedo-texture", luly::assets::asset_type::texture, albedo_texture);
    luly::renderer::material_texture albedo = {albedo_texture, luly::renderer::material_texture_type::albedo, true};

    const std::shared_ptr<luly::renderer::texture_2d>& normal_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-normal.png");
    const std::shared_ptr<luly::assets::asset>& normal_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-normal-texture", luly::assets::asset_type::texture, normal_texture);
    luly::renderer::material_texture normal = {normal_texture, luly::renderer::material_texture_type::normal, true};

    const std::shared_ptr<luly::renderer::texture_2d>& metallic_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-metallic.png");
    const std::shared_ptr<luly::assets::asset>& metallic_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-metallic-texture", luly::assets::asset_type::texture, metallic_texture);
    luly::renderer::material_texture metallic = {
        metallic_texture, luly::renderer::material_texture_type::metallic, true
    };

    const std::shared_ptr<luly::renderer::texture_2d>& roughness_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-roughness.png");
    const std::shared_ptr<luly::assets::asset>& roughness_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-roughness-texture", luly::assets::asset_type::texture, roughness_texture);
    luly::renderer::material_texture roughness = {
        roughness_texture, luly::renderer::material_texture_type::roughness, true
    };

    const std::shared_ptr<luly::renderer::texture_2d>& ao_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-ao.png");
    const std::shared_ptr<luly::assets::asset>& ao_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-ao-texture", luly::assets::asset_type::texture, ao_texture);
    luly::renderer::material_texture ao = {
        ao_texture, luly::renderer::material_texture_type::ambient_occlusion, true
    };

    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});
    textures.insert({luly::renderer::material_texture_type::ambient_occlusion, ao});

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->
        with_textures(textures).build();

    const std::shared_ptr<luly::renderer::material>& tv_material = std::make_shared<luly::renderer::material>(
        material_specification);

    const std::shared_ptr<luly::assets::asset>& tv_material_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::material>(
        "tv-material", luly::assets::asset_type::material, tv_material);

    m_actor->add_component<luly::scene::material_component>(tv_material_asset->get_data<luly::renderer::material>());
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
