#include "lypch.h"
#include "scene_renderer.h"

#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/renderer.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "application/application.h"

#include <logging/log.h>

#include "scene/actor/components/rendering/skybox_component.h"

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
        auto& current_scene = core::application::get().get_scene_manager()->get_current_scene();
        if (!current_scene) return;

        /* Camera setup */
        s_data.camera = camera;
        update_camera_data();
        update_camera_buffer();
        s_data.camera_ubo->bind(0);

        /* Lights */
        s_data.lighting_pass->update_lights();

        perform_geometry_pass();
        perform_lighting_pass();

        s_data.skybox_pass->execute();

        perform_final_pass();
    }

    void scene_renderer::end_render()
    {
    }

    void scene_renderer::set_environment_map(const std::shared_ptr<texture_2d>& environment_map)
    {
        s_data.environment_pass->set_environment_map(environment_map);
    }

    scene_renderer_data& scene_renderer::get_data()
    {
        return s_data;
    }

    void scene_renderer::create_pipeline_passes()
    {
        LY_TRACE("Started creating pipeling passes...");
        s_data.geometry_pass = std::make_shared<geometry_pass>();

        s_data.environment_pass = std::make_shared<environment_pass>();
        s_data.environment_pass->add_input({s_data.geometry_pass, "geometry_pass_input"});

        s_data.lighting_pass = std::make_shared<lighting_pass>();
        s_data.lighting_pass->add_input({s_data.geometry_pass, "geometry_pass_input"});
        s_data.lighting_pass->add_input({s_data.environment_pass, "environment_pass_input"});

        s_data.skybox_pass = std::make_shared<skybox_pass>();
        s_data.skybox_pass->add_input({s_data.environment_pass, "environment_pass_input"});
        s_data.skybox_pass->add_input({s_data.geometry_pass, "geometry_pass_input"});
        s_data.skybox_pass->add_input({s_data.lighting_pass, "lighting_pass_input"});

        s_data.final_pass = std::make_shared<final_pass>();
        s_data.final_pass->add_input({s_data.skybox_pass, "skybox_pass_input"});
        LY_TRACE("Pipeline passes created successfully!");
    }

    void scene_renderer::create_camera_data()
    {
        s_data.camera_ubo = std::make_shared<uniform_buffer_object>(sizeof(camera_data), 0);
    }

    void scene_renderer::create_common_data()
    {
    }

    void scene_renderer::update_camera_data()
    {
        s_data.camera_data.view_matrix = s_data.camera->get_view_matrix();
        s_data.camera_data.projection_matrix = s_data.camera->get_projection_matrix();
        s_data.camera_data.position = s_data.camera->get_position();
    }

    void scene_renderer::update_camera_buffer()
    {
        s_data.camera_ubo->set_data(&s_data.camera_data, sizeof(camera_data));
    }

    void scene_renderer::perform_geometry_pass()
    {
        s_data.geometry_pass->execute();
    }

    void scene_renderer::perform_lighting_pass()
    {
        s_data.lighting_pass->execute();
    }

    void scene_renderer::perform_final_pass()
    {
        s_data.final_pass->execute();
    }
}
