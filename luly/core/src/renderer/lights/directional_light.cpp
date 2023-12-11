#include "lypch.h"
#include "directional_light.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "renderer/renderer/renderer.h"
#include "renderer/renderer/pipeline/shadows/directional_light_shadows_manager.h"
#include "scene/scene.h"

namespace luly::renderer
{
    directional_light::directional_light(const glm::vec3& color, float azimuth, float elevation)
    {
        m_direction_angles = glm::vec2(azimuth, elevation);
        m_cascade_split_lambda = 0.55f;
        m_shadow_map_dimensions = glm::ivec2(4096, 4096);
        set_direction(azimuth, elevation);

        // Create shadow map fbo
        create_shadow_fbo();

        m_light_matrices_ubo = std::make_shared<uniform_buffer_object>(sizeof(glm::mat4) * CASCADES_COUNT,
                                                                       DIRECTIONAL_LIGHT_LIGHT_MATRICES_UBO_LOCATION);
        m_frustum_planes_ubo = std::make_shared<uniform_buffer_object>(sizeof(glm::vec2) * CASCADES_COUNT,
                                                                       DIRECTIONAL_LIGHT_FRUSTUM_PLANES_UBO_LOCATION);
    }

    void directional_light::calculate_cascade_splits(const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        m_shadow_cascade_splits_distances.clear();
        m_shadow_cascade_splits_distances.resize(CASCADES_COUNT);

        const float near_clip = perspective_camera->get_near_clip();
        const float far_clip = perspective_camera->get_far_clip();
        const float clip_range = far_clip - near_clip;
        const float ratio = far_clip / near_clip;

        for (int i = 0; i < CASCADES_COUNT; ++i)
        {
            float p = (i + 1) / float(CASCADES_COUNT);
            float log = near_clip * std::pow(ratio, p);
            float uni = near_clip + clip_range * p;
            float d = m_cascade_split_lambda * (log - uni) + uni;

            m_shadow_cascade_splits_distances[i] = (d - near_clip) / clip_range; // to [0, 1] range
        }
    }

    void directional_light::set_direction(float azimuth, float elevation)
    {
        m_direction_angles = {azimuth, elevation};

        float az = glm::radians(azimuth);
        float el = glm::radians(elevation);

        m_direction.x = glm::sin(el) * glm::cos(az);
        m_direction.y = glm::cos(el);
        m_direction.z = glm::sin(el) * glm::sin(az);

        m_direction = glm::normalize(-m_direction);
    }

    void directional_light::set_shadow_map_dimensions(const glm::ivec2& shadow_map_dimensions)
    {
        m_shadow_map_dimensions = shadow_map_dimensions;
        create_shadow_fbo();
    }

    void directional_light::update_shadow_cascades(const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        calculate_cascade_splits(perspective_camera);
        const std::vector<cascade_frustum>& cascade_frustums = calculate_shadow_frustums(perspective_camera);
        update_ubos(cascade_frustums);
    }

    void directional_light::create_shadow_fbo()
    {
        glDeleteTextures(1, &m_shadow_maps);

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_shadow_maps);
        glTextureStorage3D(m_shadow_maps, 1, GL_DEPTH_COMPONENT32F, m_shadow_map_dimensions.x,
                           m_shadow_map_dimensions.y, CASCADES_COUNT);

        glTextureParameteri(m_shadow_maps, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(m_shadow_maps, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_shadow_maps, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTextureParameteri(m_shadow_maps, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        constexpr float border_color[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glTextureParameterfv(m_shadow_maps, GL_TEXTURE_BORDER_COLOR, border_color);

        m_shadow_map_fbo = std::make_shared<frame_buffer>(m_shadow_map_dimensions.x, m_shadow_map_dimensions.y);
        m_shadow_map_fbo->bind();
        m_shadow_map_fbo->attach_depth_texture(m_shadow_maps, 0);
        m_shadow_map_fbo->initialize();
    }

    std::vector<cascade_frustum> directional_light::calculate_shadow_frustums(
        const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        std::vector<cascade_frustum> cascade_frustums;

        const float near_clip = perspective_camera->get_near_clip();
        const float far_clip = perspective_camera->get_far_clip();
        const float clip_range = far_clip - near_clip;

        float last_split_dist = 0.0;
        m_average_frustum_size = 0.0;

        for (uint32_t i = 0; i < CASCADES_COUNT; ++i)
        {
            float split_dist = m_shadow_cascade_splits_distances[i];

            glm::vec3 frustum_corners[8] = {
                glm::vec3(-1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, 1.0f, -1.0f),
                glm::vec3(1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, -1.0f, -1.0f),
                glm::vec3(-1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, -1.0f, 1.0f),
                glm::vec3(-1.0f, -1.0f, 1.0f),
            };

            glm::mat4 inv_cam = glm::inverse(
                perspective_camera->get_projection_matrix() * perspective_camera->get_view_matrix());
            for (uint32_t i = 0; i < 8; ++i)
            {
                glm::vec4 corner_world_space = inv_cam * glm::vec4(frustum_corners[i], 1.0f);
                frustum_corners[i] = corner_world_space / corner_world_space.w;
            }

            for (uint32_t i = 0; i < 4; ++i)
            {
                glm::vec3 dist = frustum_corners[i + 4] - frustum_corners[i];
                frustum_corners[i + 4] = frustum_corners[i] + (dist * split_dist);
                frustum_corners[i] = frustum_corners[i] + (dist * last_split_dist);
            }

            glm::vec3 frustum_center = glm::vec3(0.0f);
            for (uint32_t i = 0; i < 8; ++i)
            {
                frustum_center += frustum_corners[i];
            }
            frustum_center /= 8.0f;

            float radius = 0.0f;
            for (uint32_t i = 0; i < 8; ++i)
            {
                float distance = glm::length(frustum_corners[i] - frustum_center);
                radius = glm::max(radius, distance);
            }
            radius = ceilf(radius * 16.0f) / 16.0f;

            glm::vec3 max_extents = glm::vec3(radius);
            glm::vec3 min_extents = -max_extents;

            glm::mat4 light_view_matrix = glm::lookAt(frustum_center - m_direction * -min_extents.z, frustum_center,
                                                      glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 light_ortho_matrix = glm::ortho(min_extents.x, max_extents.x, min_extents.y, max_extents.y,
                                                      0.0f,
                                                      max_extents.z - min_extents.z);

            float split_depth = (near_clip + split_dist * clip_range) * -1.0f;

            m_average_frustum_size = glm::max(m_average_frustum_size, max_extents.x - min_extents.x);

            // Store cascade frustum data into the vector.
            cascade_frustum cascade_frustum;
            cascade_frustum.light_view_matrix = light_view_matrix;
            cascade_frustum.light_view_projection_matrix = light_ortho_matrix * light_view_matrix;
            cascade_frustum.frustum_planes = glm::vec2(min_extents.z, max_extents.z);
            cascade_frustum.split_depth = split_depth;

            // Stable csm.
            glm::vec4 shadow_origin = glm::vec4(0.0, 0.0, 0.0, 1.0);
            shadow_origin = cascade_frustum.light_view_projection_matrix * shadow_origin;
            shadow_origin = shadow_origin * (m_shadow_map_dimensions.x / 2.0f);

            glm::vec4 rounded_origin = glm::round(shadow_origin);
            glm::vec4 round_offset = rounded_origin - shadow_origin;
            round_offset = round_offset * (2.0f / m_shadow_map_dimensions.x);
            round_offset.z = 0.0f;
            round_offset.w = 0.0f;

            glm::mat4& shadow_proj = light_ortho_matrix;
            shadow_proj[3] += round_offset;

            // Update light view projection matrix after stable csm calculation. 
            cascade_frustum.light_view_projection_matrix = shadow_proj * light_view_matrix;

            // Store frustum data.
            cascade_frustums.push_back(cascade_frustum);

            last_split_dist = split_dist;
        }

        return cascade_frustums;
    }

    void directional_light::update_ubos(const std::vector<cascade_frustum>& cascade_frustums)
    {
        m_shadow_cascade_splits.clear();

        for (uint32_t i = 0; i < CASCADES_COUNT; ++i)
        {
            cascade_frustum cascade_frustum = cascade_frustums[i];
            m_shadow_cascade_splits.push_back(cascade_frustum.split_depth);

            m_light_matrices_ubo->set_data(&cascade_frustum.light_view_projection_matrix, sizeof(glm::mat4x4),
                                           i * sizeof(glm::mat4x4));
            m_frustum_planes_ubo->set_data(&cascade_frustum.frustum_planes, sizeof(glm::vec2),
                                           i * sizeof(glm::vec2));
        }
    }
}
