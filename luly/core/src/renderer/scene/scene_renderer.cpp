﻿#include "lypch.h"
#include "scene_renderer.h"

#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/model_renderer_component.h"
#include "scene/actor/components/transform_component.h"
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
        s_data.geometry_pass = std::make_shared<geometry_pass>();
        s_data.geometry_shader = shader_factory::create_shader_from_file("assets/shaders/geometry_pass_shader.lsh");
        LY_TRACE("Pipeline passes created successfully!");
    }

    void scene_renderer::create_camera_data()
    {
        s_data.camera_ubo = std::make_shared<uniform_buffer_object>();
        s_data.camera_ubo->initialize(sizeof(camera_data), nullptr);
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

        const auto& registry = current_scene->get_registry();
        const auto& view = registry->view<scene::transform_component>();

        s_data.geometry_shader->bind();

        for (auto [actor, transform_component] : view.each())
        {
            auto& transform = transform_component.get_transform();

            // Check if has model_renderer_component
            if (registry->any_of<scene::model_renderer_component>(actor))
            {
                auto& model_renderer_component = registry->get<scene::model_renderer_component>(actor);
                auto& model = model_renderer_component.get_model();

                s_data.geometry_shader->set_mat4("u_transform", transform->get_transform());
                renderer::submit_model(model);
            }
        }

        s_data.geometry_shader->un_bind();

        s_data.geometry_pass->get_frame_buffer()->un_bind();
    }
}