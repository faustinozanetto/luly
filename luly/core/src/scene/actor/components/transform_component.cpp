#include "lypch.h"
#include "transform_component.h"

namespace luly::scene
{
    transform_component::transform_component(const std::shared_ptr<math::transform>& transform)
    {
        m_transform = transform;
    }

    transform_component::transform_component(const glm::vec3& location, const glm::quat& rotation,
                                             const glm::vec3& scale)
    {
        m_transform = std::make_shared<math::transform>(location, rotation, scale);
    }

    transform_component::~transform_component()
    {
    }

    void transform_component::initialize()
    {
    }
}
