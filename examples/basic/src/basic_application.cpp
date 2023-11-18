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

    m_model = luly::renderer::model_factory::create_model_from_file("assets/models/cube.obj");

    m_texture = luly::renderer::texture_factory::create_texture_from_file("assets/textures/test.png");
    setup_triangle();
    setup_fbo();
}

basic_application::~basic_application()
{
}

void basic_application::on_update()
{
    m_shader->bind();
    glBindTextureUnit(0, m_texture->get_handle_id());
    luly::renderer::renderer::submit_model(m_model);
    m_shader->un_bind();

    ImGui::ShowDemoWindow();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
}

void basic_application::setup_triangle()
{
    const std::vector vertices = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f
    };
    const size_t size = vertices.size() * sizeof(vertices.data());

    m_triangle_vao = std::make_shared<luly::renderer::vertex_array_object>();
    m_triangle_vao->bind();

    const std::shared_ptr<luly::renderer::vertex_buffer_object> triangle_vbo = std::make_shared<
        luly::renderer::vertex_buffer_object>();
    triangle_vbo->set_data(luly::renderer::vertex_buffer_object_usage::static_draw, size, vertices.data());

    std::initializer_list<luly::renderer::vertex_buffer_layout_entry>
        layout_elements = {
            {"a_pos", luly::renderer::vertex_buffer_entry_type::FLOAT3, false},
            {"a_tex_coord", luly::renderer::vertex_buffer_entry_type::FLOAT2, false}
        };

    const std::shared_ptr<luly::renderer::vertex_buffer_layout_descriptor> vbo_layout_descriptor = std::make_shared<
        luly::renderer::vertex_buffer_layout_descriptor>(layout_elements);
    triangle_vbo->set_layout_descriptor(vbo_layout_descriptor);

    m_triangle_vao->add_vertex_buffer(triangle_vbo);
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

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1280, 720
    };
    return new basic_application(window_specification);
}
