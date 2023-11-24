#include "lypch.h"
#include "point_light.h"

namespace luly::renderer
{
    point_light::point_light(const glm::vec3& color, const glm::vec3& position) : light(color)
    {
        m_position = position;
    }
}
