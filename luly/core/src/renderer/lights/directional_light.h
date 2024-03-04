#pragma once

#include "light.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/camera/perspective/perspective_camera.h"
#include "renderer/framebuffer/frame_buffer.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    struct cascade_frustum
    {
        glm::mat4 light_view_matrix;
        glm::mat4 light_space_matrices;
        glm::vec2 frustum_planes;
        float split_depth;
    };

    class directional_light : public light
    {
    public:
        directional_light(const glm::vec3& color = glm::vec3(0.85f), float azimuth = -35.0f, float elevation = 65.0f);

        /* Getters */
        const glm::vec3& get_direction() const { return m_direction; }
        const glm::vec2& get_direction_angles() const { return m_direction_angles; }
        const std::vector<float>& get_shadow_cascade_split_distances() { return m_shadow_cascade_splits_distances; }
        const std::vector<float>& get_shadow_cascade_splits() { return m_shadow_cascade_splits; }
        const std::shared_ptr<frame_buffer>& get_shadow_map_fbo() const { return m_shadow_map_fbo; }
        uint32_t get_shadow_maps() const { return m_shadow_maps; }
        float get_cascade_split_lambda() const { return m_cascade_split_lambda; }
        const glm::ivec2& get_shadow_map_dimensions() const { return m_shadow_map_dimensions; }

        /* Setters */
        void set_direction(float azimuth, float elevation);
        void set_shadow_map_dimensions(const glm::ivec2& shadow_map_dimensions);
        void set_cascade_split_lambda(float cascade_split_lambda) { m_cascade_split_lambda = cascade_split_lambda; }

        /* Methods */
        void update_shadow_cascades(const std::shared_ptr<shader>& directional_light_shadows_shader,
                                    const std::shared_ptr<perspective_camera>& perspective_camera);
        void upload_light_space_matrices(const std::shared_ptr<shader>& shader) const;

    private:
        void create_shadow_fbo();
        void calculate_cascade_splits(const std::shared_ptr<perspective_camera>& perspective_camera);
        std::vector<cascade_frustum> calculate_shadow_frustums(
            const std::shared_ptr<perspective_camera>& perspective_camera);

        // Parameters
        glm::vec3 m_direction;
        glm::vec2 m_direction_angles;

        // Cascaded shadows
        std::vector<cascade_frustum> m_cascade_frustums;
        std::vector<float> m_shadow_cascade_splits_distances;
        std::vector<float> m_shadow_cascade_splits;
        float m_cascade_split_lambda;
        float m_average_frustum_size;
        glm::ivec2 m_shadow_map_dimensions;
        std::shared_ptr<frame_buffer> m_shadow_map_fbo;
        uint32_t m_shadow_maps;
        std::shared_ptr<uniform_buffer_object> m_frustum_planes_ubo;
    };
}
