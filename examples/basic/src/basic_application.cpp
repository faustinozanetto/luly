#include "basic_application.h"

#include <application/entry_point.h>

#include <ui_api.h>
#include "engine_ui.h"
#include "imgui.h"
#include "models/model_factory.h"
#include "renderer/renderer.h"
#include "shaders/shader.h"
#include "shaders/shader_factory.h"
#include "textures/texture_factory.h"
#include "scene/scene.h"
#include "scene/actor/components/model_renderer_component.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "scene/actor/components/transform_component.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    luly::ui::engine_ui::initialize();

    m_shader = luly::renderer::shader_factory::create_shader_from_file(
        "assets/shaders/test_shader.lsh");
    m_model = luly::renderer::model_factory::create_model_from_file("assets/models/gameboy.obj");
    m_texture = luly::renderer::texture_factory::create_texture_from_file("assets/textures/gameboy.png");

    setup_fbo();
    setup_scene();

    luly::ui::engine_ui::set_render_target(m_fbo->get_attachment_id(0));

}

basic_application::~basic_application()
{
}

void basic_application::on_create()
{
}

void basic_application::on_update()
{
    if (!get_scene_manager()->get_current_scene())
        return;

    auto& camera = get_scene_manager()->get_current_scene()->get_camera_manager()->get_perspective_camera();

    m_fbo->bind();
    luly::renderer::renderer::clear_screen();
    m_shader->bind();
    luly::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    m_shader->set_mat4("u_model_matrix",
                       m_actor->get_component<luly::scene::transform_component>().get_transform()->get_transform());
    m_shader->set_mat4("u_view_matrix", camera->get_view_matrix());
    m_shader->set_mat4("u_projection_matrix", camera->get_projection_matrix());
    luly::renderer::renderer::submit_model(m_actor->get_component<luly::scene::model_renderer_component>().get_model());
    m_shader->un_bind();
    m_fbo->un_bind();

    luly::ui::engine_ui::on_update();
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

void basic_application::setup_fbo()
{
    auto viewport_size = luly::renderer::renderer::get_viewport_size();

    std::vector<luly::renderer::frame_buffer_attachment> attachments = {
        {
            luly::renderer::texture_internal_format::rgba8,
            luly::renderer::texture_filtering::linear,
            luly::renderer::texture_wrapping::clamp_to_edge, viewport_size
        },
    };

    luly::renderer::frame_buffer_attachment depth_attachment = {
        luly::renderer::texture_internal_format::depth_component32f,
        luly::renderer::texture_filtering::linear,
        luly::renderer::texture_wrapping::clamp_to_edge, viewport_size
    };

    m_fbo = std::make_shared<luly::renderer::frame_buffer>(
        viewport_size.x, viewport_size.y, attachments, depth_attachment);
}

void basic_application::setup_scene()
{
    const auto& scene = std::make_shared<luly::scene::scene>("Test Scene");
    get_scene_manager()->add_scene(scene);
    get_scene_manager()->switch_scene(scene);

    m_actor = scene->create_actor("Test Model");
    m_actor->add_component<luly::scene::model_renderer_component>(m_model);
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
