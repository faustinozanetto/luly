#pragma once

#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/camera/camera.h"
#include "renderer/shaders/shader.h"

#include <memory>

#include "renderer/renderer/pipeline/environment_pass.h"
#include "renderer/renderer/pipeline/lighting_pass.h"
#include "renderer/renderer/pipeline/skybox_pass.h"

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
        /* Geometry Pass */
        std::shared_ptr<geometry_pass> geometry_pass;

        /* Environment Pass */
        std::shared_ptr<environment_pass> environment_pass;

        /* Lighting Pass */
        std::shared_ptr<lighting_pass> lighting_pass;

        std::shared_ptr<skybox_pass> skybox_pass;

        /* Final Pass */
        std::shared_ptr<final_pass> final_pass;

        /* Camera */
        camera_data camera_data;
        std::shared_ptr<camera> camera;
        std::shared_ptr<uniform_buffer_object> camera_ubo;
    };

    class scene_renderer
    {
    public:
        /* Methods */
        static void initialize();
        static void begin_render(const std::shared_ptr<camera>& camera);
        static void end_render();

        static void set_environment_map(const std::shared_ptr<texture_2d>& environment_map);

        /* Getters */
        static scene_renderer_data& get_data();

    private:
        static void create_pipeline_passes();
        static void create_camera_data();
        static void create_common_data();

        static void update_camera_data();
        static void update_camera_buffer();

        static void perform_geometry_pass();
        static void perform_lighting_pass();
        static void perform_final_pass();

        static scene_renderer_data s_data;
    };
}
