#include "lypch.h"
#include "point_light.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace luly::renderer
{
    point_light::point_light(const glm::vec3& color, const glm::vec3& position) : light(color)
    {
        m_position = position;

        m_constant_factor = 1.0f;
        m_linear_factor = 0.09f;
        m_quadratic_factor = 0.032f;
        m_shadow_map_dimensions = glm::ivec2(2048, 2048);
        m_shadow_map_near_plane = 0.1f;
        m_shadow_map_far_plane = 30.0f;
        m_shadow_map_projection = glm::perspective(glm::radians(90.0f), 1.0f, m_shadow_map_near_plane,
                                                   m_shadow_map_far_plane);

        create_shadow_fbo();
    }

    void point_light::set_shadow_map_far_plane(float shadow_map_far_plane)
    {
        m_shadow_map_far_plane = shadow_map_far_plane;
        m_shadow_map_projection = glm::perspective(glm::radians(90.0f), 1.0f, m_shadow_map_near_plane,
                                                   m_shadow_map_far_plane);
    }

    void point_light::update_shadow_transforms(const glm::vec3& position)
    {
        m_shadow_transforms.clear();

        m_shadow_transforms.push_back(m_shadow_map_projection *
            glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0)
                        , glm::vec3(0.0, -1.0, 0.0)));
        m_shadow_transforms.push_back(m_shadow_map_projection *
            glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0)
                        , glm::vec3(0.0, -1.0, 0.0)));
        m_shadow_transforms.push_back(m_shadow_map_projection *
            glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0)
                        , glm::vec3(0.0, 0.0, 1.0)));
        m_shadow_transforms.push_back(m_shadow_map_projection *
            glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0)
                        , glm::vec3(0.0, 0.0, -1.0)));
        m_shadow_transforms.push_back(m_shadow_map_projection *
            glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0)
                        , glm::vec3(0.0, -1.0, 0.0)));
        m_shadow_transforms.push_back(m_shadow_map_projection *
            glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0)
                        , glm::vec3(0.0, -1.0, 0.0)));
    }

    void point_light::create_shadow_fbo()
    {
        glGenTextures(1, &m_shadow_cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadow_cubemap);

        for (uint32_t i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_shadow_map_dimensions.x,
                         m_shadow_map_dimensions.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        // Create shadow map fbo and attach depth cubemap texture.
        m_shadow_map_fbo = std::make_shared<frame_buffer>(m_shadow_map_dimensions.x, m_shadow_map_dimensions.y);
        m_shadow_map_fbo->bind();
        m_shadow_map_fbo->attach_depth_texture(m_shadow_cubemap, 0);
        m_shadow_map_fbo->initialize();
    }
}
