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
        m_soft_shadows = true;
        m_z_multiplier = 20.0f;
        m_shadow_bias = 0.005f;
        m_inverse_cascade_factor = 0.003f;
        m_shadow_map_dimensions = glm::ivec2(8192, 8192);

        calculate_shadow_map_levels(70.0f);

        // Create shadow map fbo
        glGenFramebuffers(1, &m_shadow_map_fbo);
        glGenTextures(1, &m_shadow_maps);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_shadow_maps);
        glTexImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            GL_DEPTH_COMPONENT32F,
            m_shadow_map_dimensions.x,
            m_shadow_map_dimensions.y,
            int(m_shadow_cascade_levels.size()) + 1,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            nullptr);

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

    void directional_light::update_shadow_cascades(const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        float min_distance = 0.0f;
        float near_clip = perspective_camera->get_near_clip();
        float far_clip = perspective_camera->get_far_clip();
        m_shadow_data = calculate_shadow_matrix(perspective_camera);

        const glm::mat3 shadow_offset_matrix = glm::mat3(glm::transpose(m_shadow_data.shadow_view));
        for (uint32_t cascade = 0; cascade <= m_shadow_cascade_levels.size(); cascade++)
        {
            const float near_split_distance = near_clip + (cascade == 0
                                                               ? min_distance
                                                               : m_shadow_cascade_levels[cascade - 1]);
            const float far_split_distance = near_clip + (cascade == m_shadow_cascade_levels.size()
                                                              ? far_clip
                                                              : m_shadow_cascade_levels[cascade]);

            const bounding_sphere bounding_sphere = calculate_frustum_bounding_sphere(
                perspective_camera, near_split_distance, far_split_distance);

            glm::vec3 offset;

            if (cascade_needs_update(m_shadow_data.shadow_view, bounding_sphere.frustum_center,
                                     m_cascade_bound_centers[cascade], bounding_sphere.radius, offset))
            {
                glm::vec3 world_space_offset = shadow_offset_matrix * offset;
                m_cascade_bound_centers[cascade] += world_space_offset;
            }

            glm::vec3 cascade_center_shadow_space = glm::vec3(
                m_shadow_data.world_to_shadow_matrix * glm::vec4(m_cascade_bound_centers[cascade], 1.0f));
            float scale = m_shadow_data.radius / bounding_sphere.radius;

            glm::mat4 cascade_translate_inverse = glm::translate(glm::mat4(1.0f),
                                                                 glm::vec3(-cascade_center_shadow_space.x,
                                                                           -cascade_center_shadow_space.y, 0.0f));
            glm::mat4 cascade_scale = glm::scale(glm::mat4(1.0), glm::vec3(scale, scale, 1.0f));

            glm::mat4 light_view_projection_matrix = cascade_scale * cascade_translate_inverse * m_shadow_data.
                world_to_shadow_matrix;

            m_light_matrices_ubo->set_data(&light_view_projection_matrix, sizeof(glm::mat4x4),
                                           cascade * sizeof(glm::mat4x4));
        }
    }

    directional_light_shadow directional_light::calculate_shadow_matrix(
        const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        bounding_sphere shadow_bounds = calculate_frustum_bounding_sphere(
            perspective_camera, perspective_camera->get_near_clip(), perspective_camera->get_far_clip());

        const glm::vec3 camera_frustum_center_world_space = shadow_bounds.frustum_center;
        const glm::vec3 light_position = camera_frustum_center_world_space + glm::normalize(m_direction) * m_distance;

        glm::vec3 up_vector = glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

        const float direction_bias = 0.0001f;

        if (glm::abs(glm::dot(up_vector, m_direction)) < direction_bias)
        {
            up_vector = glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 0.0f, -1.0f)));
        }

        const glm::mat4 shadow_view = glm::lookAt(camera_frustum_center_world_space, light_position, up_vector);
        const glm::mat4 shadow_projection = glm::ortho(-shadow_bounds.radius, shadow_bounds.radius,
                                                       -shadow_bounds.radius,
                                                       shadow_bounds.radius, -shadow_bounds.radius,
                                                       shadow_bounds.radius);

        directional_light_shadow result;
        result.world_to_shadow_matrix = shadow_projection * shadow_view;
        result.shadow_view = shadow_view;
        result.radius = shadow_bounds.radius;
        return result;
    }

    bounding_sphere directional_light::calculate_frustum_bounding_sphere(
        const std::shared_ptr<perspective_camera>& perspective_camera, float near_clip, float far_clip)
    {
        const glm::ivec2& viewport_size = renderer::get_viewport_size();
        const float aspect_ratio = static_cast<float>(viewport_size.x) / static_cast<float>(viewport_size.y);

        std::vector<glm::vec4> corners = perspective_camera->get_frustum_corners_world_space(
            glm::perspective(glm::radians(glm::radians(perspective_camera->get_fov())), aspect_ratio,
                             near_clip, far_clip),
            perspective_camera->get_view_matrix());

        glm::vec3 center = glm::vec3(0.0f);
        for (const glm::vec4& corner : corners)
        {
            center += glm::vec3(corner);
        }
        center /= corners.size();

        float radius = 0.0f;
        for (uint32_t i = 0; i < 8; i++)
        {
            float distance = glm::length(glm::vec3(corners[i].x, corners[i].y, corners[i].z) - center);
            radius = glm::max(radius, distance);
        }
        radius = std::round(radius * 16.0f) / 16.0f;

        bounding_sphere result;
        result.frustum_center = center;
        result.radius = radius;

        return result;
    }

    bool directional_light::cascade_needs_update(const glm::mat4& shadow_view_matrix, const glm::vec3& new_center,
                                                 const glm::vec3& old_center, float radius,
                                                 glm::vec3& offset)
    {
        const glm::vec3 old_center_view_space = glm::vec3(shadow_view_matrix * glm::vec4(old_center, 1.0f));
        const glm::vec3 new_center_view_space = glm::vec3(shadow_view_matrix * glm::vec4(new_center, 1.0f));
        const glm::vec3 center_diff = new_center_view_space - old_center_view_space;

        float pixel_size = (float)m_shadow_map_dimensions.x / (2.0f * radius);

        float pixelOffsetX = center_diff.x * pixel_size;
        float pixelOffsetY = center_diff.y * pixel_size;

        const bool needs_update = glm::abs(pixelOffsetX) > 0.5f || glm::abs(pixelOffsetY) > 0.5f;
        if (needs_update)
        {
            offset.x = glm::floor(0.5f + pixelOffsetX) / pixel_size;
            offset.y = glm::floor(0.5f + pixelOffsetY) / pixel_size;
            offset.z = center_diff.z;
        }

        return needs_update;
    }

    glm::mat4 directional_light::get_light_space_matrix(const std::shared_ptr<perspective_camera>& perspective_camera,
                                                        float near_clip, float far_clip)
    {
        const glm::ivec2& viewport_size = renderer::get_viewport_size();
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
            if (maxZ < 0.5f)
            {
                maxZ = 0.5f;
            }
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
}
