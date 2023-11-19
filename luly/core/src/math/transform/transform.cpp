#include "lypch.h"
#include "transform.h"

namespace luly::math
{
    transform::transform(const glm::vec3& location, const glm::quat& rotation, const glm::vec3& scale)
    {
        m_location = location;
        m_rotation = rotation;
        m_scale = scale;
        m_transform_dirty = false;

        recalculate_transform();
    }

    transform::~transform()
    {
    }

    const glm::mat4& transform::get_transform()
    {
        if (m_transform_dirty)
            recalculate_transform();

        return m_transform;
    }

    void transform::set_location(const glm::vec3& location)
    {
        m_location = location;
        m_transform_dirty = true;
    }

    void transform::set_rotation(const glm::quat& rotation)
    {
        m_rotation = rotation;
        m_transform_dirty = true;
    }

    void transform::set_scale(const glm::vec3& scale)
    {
        m_scale = scale;
        m_transform_dirty = true;
    }

    void transform::translate(const glm::vec3& translation)
    {
        m_location += translation;
        m_transform_dirty = true;
    }

    void transform::rotate(const glm::quat& rotation)
    {
        m_rotation = rotation * m_rotation;
        m_transform_dirty = true;
    }

    void transform::scale(const glm::vec3& scaling)
    {
        m_scale *= scaling;
        m_transform_dirty = true;
    }

    void transform::recalculate_transform()
    {
        m_transform = glm::translate(glm::mat4(1.0f), m_location) *
            glm::toMat4(m_rotation) *
            glm::scale(glm::mat4(1.0f), m_scale);
        m_transform_dirty = false;
    }
}
