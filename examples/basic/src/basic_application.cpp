#include "basic_application.h"

#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "models/model_factory.h"
#include "renderer/renderer.h"
#include "shaders/shader.h"
#include "shaders/shader_factory.h"
#include "textures/texture_factory.h"
#include "scene/scene.h"
#include "time/engine_time.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    m_shader = luly::renderer::shader_factory::create_shader_from_file(
        "assets/shaders/test_shader.lsh");

    m_model = luly::renderer::model_factory::create_model_from_file("assets/models/shaylushai.obj");

    m_texture = luly::renderer::texture_factory::create_texture_from_file("assets/textures/shaylushai.jpg");
    setup_fbo();
    setup_camera();
    setup_scene();
    luly::ui::engine_ui::set_render_target(m_fbo->get_attachment_id(0));
}

basic_application::~basic_application()
{
}

void basic_application::on_update()
{
    m_fbo->bind();
    luly::renderer::renderer::clear_screen();
    m_shader->bind();
    luly::renderer::renderer::bind_texture(0, m_texture->get_handle_id());
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, luly::time::get_time(), glm::vec3(0, 1, 0));
    m_shader->set_mat4("u_model_matrix", model);
    m_shader->set_mat4("u_view_matrix", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection_matrix", m_camera->get_projection_matrix());
    luly::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();
    m_fbo->un_bind();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
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

void basic_application::setup_camera()
{
    m_camera = std::make_shared<luly::renderer::perspective_camera>(45.0f);
    m_camera->set_position({0, 1, 3.0f});
}

void basic_application::setup_scene()
{
    const auto& scene = std::make_shared<luly::scene::scene>("Test Scene");
    get_scene_manager()->add_scene(scene);
    get_scene_manager()->switch_scene(scene);

    auto actor = scene->create_actor("Test Model");
    
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
