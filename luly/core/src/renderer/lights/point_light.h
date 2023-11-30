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
        const uint32_t get_shadow_map_fbo() const { return m_shadow_map_fbo; }
        const uint32_t get_shadow_cubemap() const { return m_shadow_cubemap; }
        const std::vector<glm::mat4>& get_shadow_transforms() const { return m_shadow_transforms; }
        float get_shadow_map_far_plane() { return m_shadow_map_far_plane; }

        /* Setters */
        void set_position(const glm::vec3& position) { m_position = position; }
        void set_shadow_map_far_plane(float shadow_map_far_plane);

        /* Methods */
        void update_shadow_transforms(const glm::vec3& position);

    private:
        glm::vec3 m_position;

        uint32_t m_shadow_map_fbo;
        uint32_t m_shadow_cubemap;
        int m_shadow_map_width;
        int m_shadow_map_height;
        float m_shadow_map_near_plane;
        float m_shadow_map_far_plane;
        glm::mat4 m_shadow_map_projection;
        std::vector<glm::mat4> m_shadow_transforms;
    };
}
