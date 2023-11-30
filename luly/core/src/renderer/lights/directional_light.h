#pragma once

#include "light.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/camera/perspective/perspective_camera.h"

namespace luly::renderer
{
    struct directional_light_shadow
    {
        glm::mat4 world_to_shadow_matrix;
        glm::mat4 shadow_view;
        float radius;
    };

    struct bounding_sphere
    {
        glm::vec3 frustum_center;
        float radius;
    };

    class directional_light : public light
    {
    public:
        directional_light(const glm::vec3& color = glm::vec3(0.85f), const glm::vec3& direction = glm::vec3(0.0f));

        /* Getters */
        const glm::vec3& get_direction() const { return m_direction; }
        uint32_t get_shadow_map_fbo() const { return m_shadow_map_fbo; }
        uint32_t get_shadow_maps() const { return m_shadow_maps; }
        const glm::ivec2& get_shadow_map_dimensions() const { return m_shadow_map_dimensions; }
        float get_distance() const { return m_distance; }
        const std::vector<float>& get_shadow_cascade_levels() const { return m_shadow_cascade_levels; }
        float get_inverse_cascade_factor() const { return m_inverse_cascade_factor; }
        float get_shadow_bias() const { return m_shadow_bias; }
        bool get_soft_shadows() const { return m_soft_shadows; }

        /* Setters */
        void set_direction(const glm::vec3& direction) { m_direction = direction; }
        void set_distance(float distance) { m_distance = distance; }

        /* Methods */
        void calculate_shadow_map_levels(float far_clip);
        void update_shadow_cascades(const std::shared_ptr<perspective_camera>& perspective_camera);
        directional_light_shadow calculate_shadow_matrix(const std::shared_ptr<perspective_camera>& perspective_camera);
        bounding_sphere calculate_frustum_bounding_sphere(const std::shared_ptr<perspective_camera>& perspective_camera,
                                                          float near_clip, float far_clip);
        bool cascade_needs_update(const glm::mat4& shadow_view_matrix, const glm::vec3& new_center,
                                  const glm::vec3& old_center, float radius,
                                  glm::vec3& offset);

    private:
        glm::vec3 m_direction;
        float m_distance;

        // Cascaded shadow maps.
        glm::ivec2 m_shadow_map_dimensions;
        std::vector<float> m_shadow_cascade_levels;
        uint32_t m_shadow_map_fbo;
        uint32_t m_shadow_maps;
        std::vector<glm::vec3> m_cascade_bound_centers;
        std::shared_ptr<uniform_buffer_object> m_light_matrices_ubo;
        directional_light_shadow m_shadow_data;
        float m_inverse_cascade_factor;
        float m_shadow_bias;
        bool m_soft_shadows;
    };
}
