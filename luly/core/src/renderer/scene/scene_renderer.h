#pragma once

#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/camera/camera.h"
#include "renderer/renderer/pipeline/ambient_occlusion_pass.h"
#include "renderer/renderer/pipeline/environment_pass.h"
#include "renderer/renderer/pipeline/lighting_pass.h"
#include "renderer/renderer/pipeline/skybox_pass.h"
#include "renderer/renderer/pipeline/debanding_pass.h"
#include "renderer/renderer/pipeline/bloom_pass.h"
#include "renderer/renderer/pipeline/tonemapping_pass.h"
#include "renderer/renderer/pipeline/shadows/shadows_pass.h"

#include <memory>

namespace luly::renderer
{
    struct camera_data
    {
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
        glm::mat4 view_projection_matrix;
        glm::vec4 position;
        float near_clip;
        float far_clip;
    };

    struct scene_renderer_data
    {
        /* Passes */
        std::shared_ptr<shadows_pass> shadows_pass;
        std::shared_ptr<geometry_pass> geometry_pass;
        std::shared_ptr<environment_pass> environment_pass;
        std::shared_ptr<lighting_pass> lighting_pass;
    //    std::shared_ptr<ambient_occlusion_pass> ambient_occlusion_pass;
        std::shared_ptr<skybox_pass> skybox_pass;
        std::shared_ptr<debanding_pass> debanding_pass;
        std::shared_ptr<bloom_pass> bloom_pass;
        std::shared_ptr<tonemapping_pass> tonemapping_pass;
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

        static void set_outputs();

        static void set_environment_map(const std::shared_ptr<texture_2d>& environment_map);
        static void on_resize(const glm::ivec2& viewport_size);

        /* Getters */
        static scene_renderer_data& get_data();

    private:
        static void create_pipeline_passes();
        static void create_camera_data();
        static void create_common_data();

        static void update_camera_data();
        static void update_camera_buffer();

        static scene_renderer_data s_data;
    };
}
