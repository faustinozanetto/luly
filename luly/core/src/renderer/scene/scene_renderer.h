#pragma once

#include "renderer/camera/camera.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/renderer/pipeline/ambient_occlusion_pass.h"
#include "renderer/renderer/pipeline/shadows/shadows_pass.h"

#include <memory>

namespace luly::renderer
{
    enum class render_pass_type
    {
        shadow_pass,
        geometry_pass,
        environment_pass,
        lighting_pass,
        skybox_pass,
        bloom_pass,
        tonemapping_pass,
        debanding_pass,
        debug_pass,
        final_pass,
    };

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
        std::unordered_map<render_pass_type, std::shared_ptr<render_pass>> passes;

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
        template <typename T, typename = std::enable_if<std::is_base_of_v<render_pass, T>>>
        static std::shared_ptr<T> get_render_pass(render_pass_type pass_type)
        {
            std::shared_ptr<render_pass> base_pass = s_data.passes.at(pass_type);

            // Attempt the dynamic_cast
            std::shared_ptr<T> derived_pass = std::dynamic_pointer_cast<T>(base_pass);
            LY_ASSERT_MSG(derived_pass, "Failed to cast render_pass to the specified type!")

            return derived_pass;
        }

    private:
        static void create_pipeline_passes();
        static void create_camera_data();
        static void create_common_data();

        static void update_camera_data();
        static void update_camera_buffer();

        static scene_renderer_data s_data;
    };
}
