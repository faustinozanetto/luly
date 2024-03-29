﻿#include "lypch.h"
#include "bounding_box.h"

namespace luly::math
{
    bounding_box::bounding_box()
    {
        m_min_extents = glm::vec3(FLT_MAX);
        m_max_extents = glm::vec3(-FLT_MAX);
    }

    bounding_box::bounding_box(const bounding_box& other)
    {
        m_min_extents = other.get_min_extents();
        m_max_extents = other.get_max_extents();
    }

    bounding_box::bounding_box(const glm::vec3& min_extents, const glm::vec3& max_extents)
    {
        m_min_extents = min_extents;
        m_max_extents = max_extents;
    }

    glm::vec3 bounding_box::get_size() const
    {
        return m_max_extents - m_min_extents;
    }

    glm::vec3 bounding_box::get_center() const
    {
        return (m_max_extents + m_min_extents) * 0.5f;
    }

    void bounding_box::set_min_extents(const glm::vec3& min_extents)
    {
        m_min_extents = min_extents;
    }

    void bounding_box::set_max_extents(const glm::vec3& max_extents)
    {
        m_max_extents = max_extents;
    }

    void bounding_box::apply_transform(const glm::mat4& transform)
    {
        const glm::vec3 updated_center = transform * glm::vec4(get_center(), 1.0f);
        const glm::vec3 edge = get_size() * 0.5f;
        const auto updated_edge = glm::vec3(
            glm::abs(transform[0][0]) * edge.x + glm::abs(transform[1][0]) * edge.y + glm::abs(
                transform[2][0]) * edge.z,
            glm::abs(transform[0][1]) * edge.x + glm::abs(transform[1][1]) * edge.y + glm::abs(
                transform[2][1]) * edge.z,
            glm::abs(transform[0][2]) * edge.x + glm::abs(transform[1][2]) * edge.y + glm::abs(
                transform[2][2]) * edge.z);

        m_min_extents = updated_center - updated_edge;
        m_max_extents = updated_center + updated_edge;
    }

    bounding_box bounding_box::get_transformed(transform& transform) const
    {
        return get_transformed(transform.get_transform());
    }

    bounding_box bounding_box::get_transformed(const glm::mat4& transform) const
    {
        bounding_box box(*this);
        box.apply_transform(transform);
        return box;
    }
};
