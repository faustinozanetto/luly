﻿#include "lypch.h"
#include "camera.h"

#include "renderer/renderer/renderer.h"

namespace luly::renderer
{
    camera::camera()
    {
        m_position = glm::vec3(0.0f);
        m_world_up = glm::vec3(0, 1, 0);
        m_front = glm::vec3(0.0f, 0.0f, -1.0f);

        m_view_matrix = glm::mat4(1.0f);
        m_projection_matrix = glm::mat4(1.0f);
        m_near_clip = 0.01f;
        m_far_clip = 100.0f;
        m_yaw = -90.0f;
        m_pitch = 0.0f;

        update_vectors();
    }

    void camera::update_vectors()
    {
        LY_PROFILE_FUNCTION;
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = normalize(front);
        // also re-calculate the Right and Up vector
        m_right = normalize(cross(m_front, m_world_up));
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = normalize(cross(m_right, m_front));
    }

    void camera::set_position(const glm::vec3& position)
    {
        LY_PROFILE_FUNCTION;
        m_position = position;
        update_view_matrix();
    }

    void camera::set_near_clip(float near_clip)
    {
        LY_PROFILE_FUNCTION;
        LY_ASSERT_MSG(near_clip > 0, "Near clip must be > 0!")
        m_near_clip = near_clip;
        update_projection_matrix(renderer::get_viewport_size());
    }

    void camera::set_far_clip(float far_clip)
    {
        LY_PROFILE_FUNCTION;
        LY_ASSERT_MSG(far_clip > 0, "Far clip must be > 0!")
        m_far_clip = far_clip;
        update_projection_matrix(renderer::get_viewport_size());
    }

    void camera::set_view_matrix(const glm::mat4& view_matrix)
    {
        m_view_matrix = view_matrix;
    }

    void camera::set_projection_matrix(const glm::mat4& projection_matrix)
    {
        m_projection_matrix = projection_matrix;
    }
}
