#include "lypch.h"
#include "spot_light.h"

namespace luly::renderer
{
    spot_light::spot_light(const glm::vec3& color, const glm::vec3& position, const glm::vec3& direction,
                           float inner_cone_angle,
                           float outer_cone_angle) : light(color)
    {
        m_position = position;
        m_direction = direction;
        m_inner_cone_angle = inner_cone_angle;
        m_outer_cone_angle = outer_cone_angle;
    }
}
