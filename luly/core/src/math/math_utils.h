#pragma once

#include <glm/glm.hpp>

namespace luly::math
{
    class math_utils
    {
    public:
        static bool decompose_transform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation,
                                        glm::vec3& scale);
    };
}
