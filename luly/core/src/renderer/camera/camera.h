#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace luly::renderer
{
    class camera
    {
    public:
        camera();
        virtual ~camera() = default;

        /* Virtuals */
        virtual void update_view_matrix() = 0;
        virtual void update_projection_matrix(const glm::ivec2& dimensions) = 0;

        /* Getters */
        const glm::mat4& get_view_matrix() const { return m_view_matrix; }
        const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
        const glm::vec3& get_position() const { return m_position; }
        const glm::vec3& get_front() const { return m_front; }
        const glm::vec3& get_up() const { return m_up; }
        const glm::vec3& get_world_up() const { return m_world_up; }
        const glm::vec3& get_right() const { return m_right; }
        float get_near_clip() const { return m_near_clip; }
        float get_far_clip() const { return m_far_clip; }
        float get_pitch() const { return m_pitch; }
        float get_yaw() const { return m_yaw; }

        /* Setters */
        void set_position(const glm::vec3& position);
        void set_pitch(float pitch) { m_pitch = pitch; }
        void set_yaw(float yaw) { m_yaw = yaw; }
        void set_near_clip(float near_clip);
        void set_far_clip(float far_clip);
        void set_view_matrix(const glm::mat4& view_matrix);
        void set_projection_matrix(const glm::mat4& projection_matrix);

        /* Methods */
        void update_vectors();

    protected:
        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_world_up;
        glm::vec3 m_right;

        float m_near_clip;
        float m_far_clip;

        float m_pitch;
        float m_yaw;
    };
}
