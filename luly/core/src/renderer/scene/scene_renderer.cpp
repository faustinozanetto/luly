#include "lypch.h"
#include "scene_renderer.h"

#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "application/application.h"

#include <logging/log.h>

namespace luly::renderer
{
    scene_renderer_data scene_renderer::s_data = {};

    void scene_renderer::initialize()
    {
        LY_TRACE("Scene renderer initialization started...");
        create_pipeline_passes();
        create_camera_data();
        create_common_data();
        LY_TRACE("Scene renderer initialized successfully!");
    }

    void scene_renderer::begin_render(const std::shared_ptr<camera>& camera)
    {
        /* Camera setup */
        s_data.camera = camera;
        update_camera_data();
        update_camera_buffer();
        s_data.camera_ubo->bind(0);

        perform_geometry_pass();
        perform_lighting_pass();
        perform_final_pass();
    }

    void scene_renderer::end_render()
    {
    }

    scene_renderer_data& scene_renderer::get_data()
    {
        return s_data;
    }

    void scene_renderer::create_pipeline_passes()
    {
        LY_TRACE("Started creating pipeling passes...");
        s_data.final_pass = std::make_shared<final_pass>();
        s_data.final_shader = shader_factory::create_shader_from_file("assets/shaders/final_pass_shader.lsh");

        s_data.geometry_pass = std::make_shared<geometry_pass>();
        s_data.geometry_shader = shader_factory::create_shader_from_file("assets/shaders/geometry_pass_shader.lsh");

        s_data.lighting_pass = std::make_shared<lighting_pass>();
        s_data.lighting_shader = shader_factory::create_shader_from_file("assets/shaders/lighting_pass_shader.lsh");
        LY_TRACE("Pipeline passes created successfully!");
    }

    void scene_renderer::create_camera_data()
    {
        s_data.camera_ubo = std::make_shared<uniform_buffer_object>();
        s_data.camera_ubo->initialize(sizeof(camera_data), nullptr);
    }

    void scene_renderer::create_common_data()
    {
        s_data.screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void scene_renderer::update_camera_data()
    {
        s_data.camera_data.view_matrix = s_data.camera->get_view_matrix();
        s_data.camera_data.projection_matrix = s_data.camera->get_projection_matrix();
        s_data.camera_data.position = s_data.camera->get_position();
    }

    void scene_renderer::update_camera_buffer()
    {
        s_data.camera_ubo->set_data(sizeof(camera_data), &s_data.camera_data);
    }

    void scene_renderer::perform_geometry_pass()
    {
        auto& current_scene = core::application::get().get_scene_manager()->get_current_scene();
        if (!current_scene) return;

        s_data.geometry_pass->get_frame_buffer()->bind();
        renderer::clear_screen();
        s_data.geometry_shader->bind();

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

                material->bind(s_data.geometry_shader);
            }

            auto& model_renderer_component = registry->get<scene::model_renderer_component>(actor);
            auto& model = model_renderer_component.get_model();

            s_data.geometry_shader->set_mat4("u_transform", transform->get_transform());
            renderer::submit_model(model);
        }

        s_data.geometry_shader->un_bind();
        s_data.geometry_pass->get_frame_buffer()->un_bind();
    }

    void scene_renderer::perform_lighting_pass()
    {
        s_data.lighting_pass->get_frame_buffer()->bind();
        renderer::clear_screen();
        s_data.lighting_shader->bind();

        // Bind geometry pass outputs.
        renderer::bind_texture(0, s_data.geometry_pass->get_frame_buffer()->get_attachment_id(0));
        renderer::bind_texture(1, s_data.geometry_pass->get_frame_buffer()->get_attachment_id(1));
        renderer::bind_texture(2, s_data.geometry_pass->get_frame_buffer()->get_attachment_id(2));
        renderer::bind_texture(3, s_data.geometry_pass->get_frame_buffer()->get_attachment_id(3));

        renderer::submit_mesh(s_data.screen_mesh);

        s_data.lighting_shader->un_bind();
        s_data.lighting_pass->get_frame_buffer()->un_bind();
    }

    void scene_renderer::perform_final_pass()
    {
        s_data.final_pass->get_frame_buffer()->bind();
        renderer::clear_screen();
        s_data.final_shader->bind();

        renderer::bind_texture(0, s_data.lighting_pass->get_frame_buffer()->get_attachment_id(0));
        renderer::submit_mesh(s_data.screen_mesh);

        s_data.final_shader->un_bind();
        s_data.final_pass->get_frame_buffer()->un_bind();
    }
}
