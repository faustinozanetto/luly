#include "lypch.h"
#include "geometry_pass.h"

#include "application/application.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

namespace luly::renderer
{
    geometry_pass::geometry_pass() : render_pass("geometry_pass")
    {
        geometry_pass::initialize();
    }

    geometry_pass::~geometry_pass()
    {
    }

    void geometry_pass::initialize()
    {
        auto viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            // Position
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
            // Albedo
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
            // Normal
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
            // Roughness-Metallic-AO
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32f,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge, viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);

        // Load shaders.
        m_geometry_shader = shader_factory::create_shader_from_file("assets/shaders/geometry_pass_shader.lsh");
    }

    void geometry_pass::execute()
    {
        m_fbo->bind();
        renderer::clear_screen();
        m_geometry_shader->bind();

        auto& current_scene = core::application::get().get_scene_manager()->get_current_scene();
        const auto& registry = current_scene->get_registry();
        const auto& view = registry->view<scene::transform_component>();

        for (auto [actor, transform_component] : view.each())
        {
            auto& transform = transform_component.get_transform();

            // Check if has model_renderer_component
            if (!registry->any_of<scene::model_renderer_component>(actor)) continue;

            // Check if has material_component
            if (registry->any_of<scene::material_component>(actor))
            {
                auto& material_component = registry->get<scene::material_component>(actor);
                auto& material = material_component.get_material();

                material->bind(m_geometry_shader);
            }

            auto& model_renderer_component = registry->get<scene::model_renderer_component>(actor);
            auto& model = model_renderer_component.get_model();

            m_geometry_shader->set_mat4("u_transform", transform->get_transform());
            renderer::submit_model(model);
        }

        m_geometry_shader->un_bind();
        m_fbo->un_bind();
    }
}
