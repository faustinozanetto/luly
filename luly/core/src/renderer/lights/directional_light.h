#pragma once

#include "light.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/camera/perspective/perspective_camera.h"

namespace luly::renderer
{
    struct cascade_frustum
    {
        glm::mat4 light_view_matrix;
        glm::mat4 light_view_projection_matrix;
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
        float get_radius() const { return m_radius; }
        const std::vector<float>& get_shadow_cascade_split_distances() { return m_shadow_cascade_splits_distances; }
        const std::vector<float>& get_shadow_cascade_splits() { return m_shadow_cascade_splits; }
        uint32_t get_shadow_map_fbo() const { return m_shadow_map_fbo; }
        uint32_t get_shadow_maps() const { return m_shadow_maps; }
        uint32_t get_shadow_map_pcf_sampler() const { return m_shadow_map_pcf_sampler; }
        const glm::ivec2& get_shadow_map_dimensions() const { return m_shadow_map_dimensions; }
        int get_cascades_count() const { return m_cascades_count; }
        const std::shared_ptr<uniform_buffer_object>& get_light_matrices_ubo() const { return m_light_matrices_ubo; }

        /* Setters */
        void set_direction(float azimuth, float elevation);
        void set_radius(float radius) { m_radius = radius; }

        /* Methods */
        void update_shadow_cascades(const std::shared_ptr<perspective_camera>& perspective_camera);

    private:
        void create_shadow_fbo();
        void calculate_cascade_splits(const std::shared_ptr<perspective_camera>& perspective_camera);
        std::vector<cascade_frustum> calculate_shadow_frustums(
            const std::shared_ptr<perspective_camera>& perspective_camera);
        void update_ubos(const std::vector<cascade_frustum>& cascade_frustums);

        glm::vec3 m_direction;
        glm::vec2 m_direction_angles;
        float m_radius;

        // Cascade Splits
        const int m_cascades_count = 6;
        std::vector<float> m_shadow_cascade_splits_distances;
        std::vector<float> m_shadow_cascade_splits;
        float m_cascade_split_lambda;
        float m_average_frustum_size;

        glm::ivec2 m_shadow_map_dimensions;
        uint32_t m_shadow_map_fbo;
        uint32_t m_shadow_map_pcf_sampler;
        uint32_t m_shadow_maps;
        std::shared_ptr<uniform_buffer_object> m_light_matrices_ubo;
        std::shared_ptr<uniform_buffer_object> m_frustum_planes_ubo;
    };
}
