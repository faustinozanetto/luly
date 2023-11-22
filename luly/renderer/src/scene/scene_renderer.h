#pragma once

#include "buffers/uniform/uniform_buffer_object.h"
#include "renderer/pipeline/final_pass.h"
#include "renderer/pipeline/geometry_pass.h"

#include <memory>

#include "camera/camera.h"

namespace luly::renderer
{
    struct camera_data
    {
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
        glm::vec3 position;
    };

    struct scene_renderer_data
    {
        /* Passes */
        std::shared_ptr<geometry_pass> geometry_pass;
        std::shared_ptr<final_pass> final_pass;

        /* Camera */
        camera_data camera_data;
        std::shared_ptr<camera> camera;
        std::shared_ptr<uniform_buffer_object> camera_ubo;
    };

    class scene_renderer
    {
    public:
        static void initialize();
        static void begin_render(const std::shared_ptr<camera>& camera);
        static void end_render();

    private:
        static void create_pipeline_passes();
        static void create_camera_data();

        static void update_camera_data();
        static void update_camera_buffer();

        static void perform_geometry_pass();

        static scene_renderer_data s_data;
    };
}
