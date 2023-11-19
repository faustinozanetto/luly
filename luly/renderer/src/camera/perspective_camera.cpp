﻿#include "perspective_camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "renderer/renderer.h"

namespace luly::renderer
{
    perspective_camera::perspective_camera(float fov)
    {
        m_fov = fov;
        perspective_camera::update_projection_matrix(renderer::get_viewport_size());
        perspective_camera::update_view_matrix();
    }

    perspective_camera::~perspective_camera()
    {
    }

    void perspective_camera::update_view_matrix()
    {
        m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
    }

    void perspective_camera::update_projection_matrix(const glm::ivec2& dimensions)
    {
        const float aspect_ratio = static_cast<float>(dimensions.x) / static_cast<float>(dimensions.y);
        m_projection_matrix = glm::perspective(glm::radians(m_fov), aspect_ratio, m_near_clip, m_far_clip);
    }
}