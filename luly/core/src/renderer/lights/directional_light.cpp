#include "lypch.h"
#include "directional_light.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "renderer/renderer/renderer.h"
#include "scene/scene.h"

namespace luly::renderer
{
    directional_light::directional_light(const glm::vec3& color, const glm::vec3& direction) : light(color)
    {
        m_direction = direction;
        m_distance = 20.0f;
        m_z_multiplier = 20.0f;
        m_shadow_map_dimensions = glm::ivec2(8196, 8196);

        calculate_shadow_map_levels(70.0f);

        // Create shadow map fbo
        glGenFramebuffers(1, &m_shadow_map_fbo);
        glGenTextures(1, &m_shadow_maps);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadow_maps);
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, m_shadow_map_dimensions.x,
                     m_shadow_map_dimensions.y, int(m_shadow_cascade_levels.size()) + 1,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        glGenTextures(m_shadow_cascade_levels.size(), m_shadow_map_views);

        constexpr float bordercolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

        glBindFramebuffer(GL_FRAMEBUFFER, m_shadow_map_fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadow_maps, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (status != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!";
            throw 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        m_light_matrices_ubo = std::make_shared<uniform_buffer_object>(sizeof(glm::mat4) * 16, 2);

        m_cascade_bound_centers.reserve(m_shadow_cascade_levels.size() + 1);
        for (uint32_t i = 0; i < m_shadow_cascade_levels.size() + 1; i++)
        {
            m_cascade_bound_centers.push_back(glm::vec3(0.0f));
        }
    }
    
    glm::mat4 directional_light::get_light_space_matrix(const std::shared_ptr<perspective_camera>& perspective_camera,
                                                        float near_clip, float far_clip)
    {
        const glm::ivec2 viewport_size = renderer::get_viewport_size();
        float aspect_ratio = static_cast<float>(viewport_size.x) / static_cast<float>(viewport_size.y);

        glm::mat4 projection_matrix = glm::perspective(
            glm::radians(perspective_camera->get_fov()), aspect_ratio, near_clip,
            far_clip);

        std::vector<glm::vec4> corners = perspective_camera->get_frustum_corners_world_space(
            projection_matrix, perspective_camera->get_view_matrix());

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const glm::vec4& corner : corners)
        {
            center += glm::vec3(corner);
        }
        center /= corners.size();

        glm::mat4 light_view_matrix = glm::lookAt(
            center - m_direction,
            center,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::min();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::min();
        float minZ = std::numeric_limits<float>::max();
        float maxZ = std::numeric_limits<float>::min();
        for (const auto& v : corners)
        {
            const auto trf = light_view_matrix * v;
            minX = std::min(minX, trf.x);
            maxX = std::max(maxX, trf.x);
            minY = std::min(minY, trf.y);
            maxY = std::max(maxY, trf.y);
            minZ = std::min(minZ, trf.z);
            maxZ = std::max(maxZ, trf.z);
        }

        if (minZ < 0)
        {
            minZ *= m_z_multiplier;
        }
        else
        {
            minZ /= m_z_multiplier;
        }
        if (maxZ < 0)
        {
            maxZ /= m_z_multiplier;
        }
        else
        {
            /*
            if (maxZ < 0.5f)
            {
                maxZ = 0.5f;
            }*/
            maxZ *= m_z_multiplier;
        }

        glm::mat4 light_projection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        return light_projection * light_view_matrix;
    }

    std::vector<glm::mat4> directional_light::get_light_space_matrices(
        const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        std::vector<glm::mat4> matrices;

        for (size_t i = 0; i < m_shadow_cascade_levels.size() + 1; ++i)
        {
            if (i == 0)
            {
                matrices.push_back(get_light_space_matrix(perspective_camera, perspective_camera->get_near_clip(),
                                                          m_shadow_cascade_levels[i]));
            }
            else if (i < m_shadow_cascade_levels.size())
            {
                matrices.push_back(get_light_space_matrix(perspective_camera, m_shadow_cascade_levels[i - 1],
                                                          m_shadow_cascade_levels[i]));
            }
            else
            {
                matrices.push_back(get_light_space_matrix(perspective_camera, m_shadow_cascade_levels[i - 1],
                                                          perspective_camera->get_far_clip()));
            }
        }
        return matrices;
    }

    void directional_light::update_shadow_map_views()
    {
        for (uint32_t i = 0; i < m_shadow_cascade_levels.size(); ++i)
        {
            glBindTexture(GL_TEXTURE_2D, m_shadow_map_views[i]);
            glTextureView(m_shadow_map_views[i], GL_TEXTURE_2D, m_shadow_maps, 0, i, 1, 1, 0);
        }
    }

    void directional_light::calculate_shadow_map_levels(float far_clip)
    {
        m_shadow_cascade_levels.clear();
        m_shadow_cascade_levels.push_back(far_clip / 50.0f);
        m_shadow_cascade_levels.push_back(far_clip / 25.0f);
        m_shadow_cascade_levels.push_back(far_clip / 10.0f);
        m_shadow_cascade_levels.push_back(far_clip / 2.0f);
    }

    void directional_light::update_shadow_cascades(const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        std::vector<glm::mat4> light_space_matrices = get_light_space_matrices(perspective_camera);
        for (size_t i = 0; i < light_space_matrices.size(); ++i)
        {
            m_light_matrices_ubo->set_data(&light_space_matrices[i], sizeof(glm::mat4x4),
                                           i * sizeof(glm::mat4x4));
        }
    }
}
