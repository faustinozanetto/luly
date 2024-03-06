#pragma once

#include "light.h"
#include "renderer/framebuffer/frame_buffer.h"

namespace luly::renderer
{
    class point_light : public light
    {
    public:
        point_light(const glm::vec3& color = glm::vec3(0.8f), const glm::vec3& position = glm::vec3(0.0f));

        /* Getters */
        const glm::vec3& get_position() const { return m_position; }
        const std::shared_ptr<frame_buffer>& get_shadow_map_fbo() const { return m_shadow_map_fbo; }
        const uint32_t get_shadow_cubemap() const { return m_shadow_cubemap; }
        const std::vector<glm::mat4>& get_shadow_transforms() const { return m_shadow_transforms; }
        float get_shadow_map_far_plane() const { return m_shadow_map_far_plane; }
        float get_shadow_map_near_plane() const { return m_shadow_map_near_plane; }
        const glm::ivec2& get_shadow_map_dimensions() const { return m_shadow_map_dimensions; }
        float get_quadratic_factor() const { return m_quadratic_factor; }
        float get_linear_factor() const { return m_linear_factor; }
        float get_constant_factor() const { return m_constant_factor; }

        /* Setters */
        void set_position(const glm::vec3& position) { m_position = position; }
        void set_shadow_map_far_plane(float shadow_map_far_plane);
        void set_quadratic_factor(float quadratic_factor) { m_quadratic_factor = quadratic_factor; }
        void set_linear_factor(float linear_factor) { m_linear_factor = linear_factor; }
        void set_constant_factor(float constant_factor) { m_constant_factor = constant_factor; }

        /* Methods */
        void update_shadow_transforms(const glm::vec3& position);

    private:
        void create_shadow_fbo();

        glm::vec3 m_position;
        float m_constant_factor;
        float m_linear_factor;
        float m_quadratic_factor;

        /* Shadow mapping */
        std::shared_ptr<frame_buffer> m_shadow_map_fbo;
        uint32_t m_shadow_cubemap;
        glm::ivec2 m_shadow_map_dimensions;
        float m_shadow_map_near_plane;
        float m_shadow_map_far_plane;
        glm::mat4 m_shadow_map_projection;
        std::vector<glm::mat4> m_shadow_transforms;
    };
}
