#include "lypch.h"
#include "perspective_camera.h"

#include "renderer/renderer/renderer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

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

    std::vector<glm::vec4> perspective_camera::get_frustum_corners_world_space(
        const glm::mat4& projection_matrix, const glm::mat4& view_matrix)
    {
        glm::mat4 inverse = glm::inverse(projection_matrix * view_matrix);

        std::vector<glm::vec4> frustum_corners;

        for (unsigned int x = 0; x < 2; ++x)
        {
            for (unsigned int y = 0; y < 2; ++y)
            {
                for (unsigned int z = 0; z < 2; ++z)
                {
                    const glm::vec4 pt = inverse * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                    frustum_corners.push_back(pt / pt.w);
                }
            }
        }

        return frustum_corners;
    }

    void perspective_camera::set_fov(float fov)
    {
        LY_ASSERT_MSG(fov > 0.0f && fov < 179.0f, "FOV must be in the range (0,180)!")
        m_fov = fov;
        update_projection_matrix(renderer::get_viewport_size());
    }
}
