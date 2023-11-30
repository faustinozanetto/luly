#include "lypch.h"
#include "point_light.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace luly::renderer
{
    point_light::point_light(const glm::vec3& color, const glm::vec3& position) : light(color)
    {
        m_position = position;

        m_shadow_map_width = 2048;
        m_shadow_map_height = 2048;
        m_shadow_map_near_plane = 0.1f;
        m_shadow_map_far_plane = 30.0f;
        m_shadow_map_projection = glm::perspective(glm::radians(90.0f), 1.0f, m_shadow_map_near_plane,
                                                   m_shadow_map_far_plane);

        glGenFramebuffers(1, &m_shadow_map_fbo);
        // create depth cubemap texture
        glGenTextures(1, &m_shadow_cubemap);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_shadow_cubemap);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_shadow_map_width,
                         m_shadow_map_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        // attach depth texture as FBO's depth buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadow_cubemap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
}
