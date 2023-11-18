#include "basic_application.h"

#include <application/entry_point.h>

#include "../../../third_party/imgui/imgui.h"
#include "models/model_factory.h"
#include "renderer/renderer.h"
#include "shaders/shader.h"
#include "shaders/shader_factory.h"
#include "textures/texture_factory.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    m_shader = luly::renderer::shader_factory::create_shader_from_file(
        "assets/shaders/test_shader.lsh");

    m_model = luly::renderer::model_factory::create_model_from_file("assets/models/monkey.obj");

    m_texture = luly::renderer::texture_factory::create_texture_from_file("assets/textures/test.png");
    setup_fbo();
    setup_camera();
}

basic_application::~basic_application()
{
}

void basic_application::on_update()
{
    m_shader->bind();
    glBindTextureUnit(0, m_texture->get_handle_id());
    m_shader->set_mat4("u_view_matrix", m_camera->get_view_matrix());
    m_shader->set_mat4("u_projection_matrix", m_camera->get_projection_matrix());
    luly::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();

    ImGui::ShowDemoWindow();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
}

void basic_application::setup_fbo()
{
    auto viewport_size = luly::renderer::renderer::get_viewport_size();

    std::vector<luly::renderer::frame_buffer_attachment> attachments = {
        {
            luly::renderer::texture_internal_format::rgba16f,
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
    m_camera->set_position({0, 0, 3.0f});
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1280, 720
    };
    return new basic_application(window_specification);
}
