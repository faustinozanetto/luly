﻿#include "camera.h"

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
        m_far_clip = 1000.0f;
        m_yaw = -90.0f;
        m_pitch = 0.0f;

        update_vectors();
    }

    void camera::update_vectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        m_right = glm::normalize(glm::cross(m_front, m_world_up));
        // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    void camera::set_position(const glm::vec3& position)
    {
        m_position = position;
        update_view_matrix();
    }
}