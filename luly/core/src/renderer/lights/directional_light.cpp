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
        m_cascades_count = 4;
        m_shadow_map_dimensions = glm::ivec2(8196, 8196);

        calculate_cascade_levels(500.0f);
        
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
        const float aspect_ratio = static_cast<float>(viewport_size.x) / static_cast<float>(viewport_size.y);

        const glm::mat4 projection_matrix = glm::perspective(
            glm::radians(perspective_camera->get_fov()), aspect_ratio, near_clip,
            far_clip);

        const std::vector<glm::vec4> corners = perspective_camera->get_frustum_corners_world_space(
            projection_matrix, perspective_camera->get_view_matrix());

        glm::vec3 center = glm::vec3(0, 0, 0);
        for (const glm::vec4& corner : corners)
        {
            center += glm::vec3(corner);
        }
        center /= corners.size();

        const glm::mat4 light_view_matrix = glm::lookAt(
            center - m_direction,
            center,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );

        float min_x = std::numeric_limits<float>::max();
        float max_x = std::numeric_limits<float>::min();
        float min_y = std::numeric_limits<float>::max();
        float max_y = std::numeric_limits<float>::min();
        float min_z = std::numeric_limits<float>::max();
        float max_z = std::numeric_limits<float>::min();
        for (const auto& v : corners)
        {
            const auto trf = light_view_matrix * v;
            min_x = std::min(min_x, trf.x);
            max_x = std::max(max_x, trf.x);
            min_y = std::min(min_y, trf.y);
            max_y = std::max(max_y, trf.y);
            min_z = std::min(min_z, trf.z);
            max_z = std::max(max_z, trf.z);
        }

        if (min_z < 0)
        {
            min_z *= m_z_multiplier;
        }
        else
        {
            min_z /= m_z_multiplier;
        }
        if (max_z < 0)
        {
            max_z /= m_z_multiplier;
        }
        else
        {
            if (max_z < 0.5f)
            {
                max_z = 0.5f;
            }
            max_z *= m_z_multiplier;
        }

        const glm::mat4 light_projection = glm::ortho(min_x, max_x, min_y, max_y, min_z, max_z);
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

    void directional_light::set_cascades_count(int cascades_count)
    {
        m_cascades_count = cascades_count;
    }

    void directional_light::update_shadow_map_views() const
    {
        for (uint32_t i = 0; i < m_shadow_cascade_levels.size(); ++i)
        {
            glBindTexture(GL_TEXTURE_2D, m_shadow_map_views[i]);
            glTextureView(m_shadow_map_views[i], GL_TEXTURE_2D, m_shadow_maps, 0, i, 1, 1, 0);
        }
    }

    void directional_light::calculate_cascade_levels(float far_clip)
    {
        m_shadow_cascade_levels.clear();
        for (int i = 0; i < m_cascades_count; ++i)
        {
            float split_factor = static_cast<float>(i + 1) / static_cast<float>(m_cascades_count + 1);
            float cascade_far_clip = std::pow(far_clip, split_factor);
            m_shadow_cascade_levels.push_back(cascade_far_clip);
        }
    }

    void directional_light::update_shadow_cascades(const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        const std::vector<glm::mat4> light_space_matrices = get_light_space_matrices(perspective_camera);
        for (size_t i = 0; i < light_space_matrices.size(); ++i)
        {
            m_light_matrices_ubo->set_data(&light_space_matrices[i], sizeof(glm::mat4x4),
                                           i * sizeof(glm::mat4x4));
        }
    }
}
