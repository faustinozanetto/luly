#include "scene_renderer.h"

#include "renderer/pipeline/final_pass.h"
#include "renderer/pipeline/geometry_pass.h"
#include "renderer/renderer.h"

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
    }

    void scene_renderer::end_render()
    {
    }

    void scene_renderer::create_pipeline_passes()
    {
        LY_TRACE("Started creating pipeling passes...");
        s_data.final_pass = std::make_shared<final_pass>();
        s_data.geometry_pass = std::make_shared<geometry_pass>();
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
        s_data.geometry_pass->get_frame_buffer()->bind();
        renderer::clear_screen();
    }
}
