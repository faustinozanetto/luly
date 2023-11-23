#include "basic_application.h"

#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "renderer/textures/texture_factory.h"
#include "scene/scene.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    luly::ui::engine_ui::initialize();

    setup_scene();

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

    const auto& actor = scene->create_actor("Test Model");
    const auto& model = luly::renderer::model_factory::create_model_from_file("assets/models/gameboy.obj");
    actor->add_component<luly::scene::model_renderer_component>(model);

    const auto& albedo_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/gameboy/DefaultMaterial_Albedo.png");
    luly::renderer::material_texture albedo;
    albedo.texture = albedo_texture;
    albedo.is_enabled = true;
    albedo.type = luly::renderer::material_texture_type::albedo;

    const auto& normal_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/gameboy/DefaultMaterial_Normal.png");
    luly::renderer::material_texture normal;
    normal.texture = normal_texture;
    normal.is_enabled = true;
    normal.type = luly::renderer::material_texture_type::normal;

    const auto& metallic_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/gameboy/DefaultMaterial_Metallic.png");
    luly::renderer::material_texture metallic;
    metallic.texture = metallic_texture;
    metallic.is_enabled = true;
    metallic.type = luly::renderer::material_texture_type::metallic;

    const auto& roughness_texture = luly::renderer::texture_factory::create_texture_from_file(
        "assets/textures/gameboy/DefaultMaterial_Rouhgness.png");
    luly::renderer::material_texture roughness;
    roughness.texture = roughness_texture;
    roughness.is_enabled = true;
    roughness.type = luly::renderer::material_texture_type::roughness;

    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});

    auto material_specification = std::make_shared<luly::renderer::material_specification_builder>()->
                                  with_textures(textures).build();

    auto material = std::make_shared<luly::renderer::material>(material_specification);
    actor->add_component<luly::scene::material_component>(material);
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
