#include "lypch.h"
#include "directional_light.h"

namespace luly::renderer
{
    directional_light::directional_light(const glm::vec3& color, const glm::vec3& direction) : light(color)
    {
        m_direction = direction;
    }
}
