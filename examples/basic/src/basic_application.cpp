﻿#include "basic_application.h"

#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/textures/texture_factory.h"
#include "scene/scene.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
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
        luly::renderer::scene_renderer::get_data().final_pass->get_frame_buffer()->get_attachment_id(0));
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

    if (!get_scene_manager()->get_current_scene())
        return;
    
    luly::ui::engine_ui::begin_frame();

    auto& camera = get_scene_manager()->get_current_scene()->get_camera_manager()->get_perspective_camera();

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
    if (!get_scene_manager()->get_current_scene()) return false;

    auto& camera_controller = get_scene_manager()->get_current_scene()->get_camera_manager()->
                                                   get_perspective_camera_controller();

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
    const auto& scene = std::make_shared<luly::scene::scene>("Test Scene");
    get_scene_manager()->add_scene(scene);
    get_scene_manager()->switch_scene(scene);

    /*
    const auto& dir_light_actor = scene->create_actor("Light Emitter");
    dir_light_actor->add_component<luly::scene::directional_light_component>(
        std::make_shared<luly::renderer::directional_light>(glm::vec3(0.85f), glm::vec3(0.8f, 0.3f, -5.0f)));

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
            -1 + i, 0, 1.5f
        });
    }
    */

    const auto& skybox_actor = scene->create_actor("Skybox Actor");
    skybox_actor->add_component<luly::scene::skybox_component>(
        luly::renderer::texture_factory::create_environment_texture_from_file(
            "assets/hdris/kart_club_4k.hdr"));

    const auto& spot_light_actor = scene->create_actor("Spot Light Emitter");
    spot_light_actor->add_component<luly::scene::spot_light_component>(
        std::make_shared<luly::renderer::spot_light>(glm::vec3(0.85f), glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), 12.5f,
                                                     25.0f));
    spot_light_actor->get_component<luly::scene::transform_component>().get_transform()->set_location({0, 0, 2});

    m_actor = scene->create_actor("Test Model");
    const auto& model = luly::renderer::model_factory::create_model_from_file("assets/models/tv/tv.obj");
    m_actor->add_component<luly::scene::model_renderer_component>(model);

    const auto& albedo_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/tv/tv-albedo.png");
    luly::renderer::material_texture albedo;
    albedo.texture = albedo_texture;
    albedo.is_enabled = true;
    albedo.type = luly::renderer::material_texture_type::albedo;

    const auto& normal_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/tv/tv-normal.png");
    luly::renderer::material_texture normal;
    normal.texture = normal_texture;
    normal.is_enabled = true;
    normal.type = luly::renderer::material_texture_type::normal;

    const auto& metallic_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/tv/tv-metallic.png");
    luly::renderer::material_texture metallic;
    metallic.texture = metallic_texture;
    metallic.is_enabled = true;
    metallic.type = luly::renderer::material_texture_type::metallic;

    const auto& roughness_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/tv/tv-roughness.png");
    luly::renderer::material_texture roughness;
    roughness.texture = roughness_texture;
    roughness.is_enabled = true;
    roughness.type = luly::renderer::material_texture_type::roughness;

    const auto& ao_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/tv/tv-ao.png");
    luly::renderer::material_texture ambient_occlusion;
    ambient_occlusion.texture = ao_texture;
    ambient_occlusion.is_enabled = true;
    ambient_occlusion.type = luly::renderer::material_texture_type::ambient_occlusion;

    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});
    textures.insert({luly::renderer::material_texture_type::ambient_occlusion, ambient_occlusion});

    auto material_specification = std::make_shared<luly::renderer::material_specification_builder>()->
                                  with_textures(textures).build();

    auto material = std::make_shared<luly::renderer::material>(material_specification);
    m_actor->add_component<luly::scene::material_component>(material);
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
