#include "lypch.h"
#include "math_utils.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace luly::math
{
    bool math_utils::decompose_transform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation,
                                         glm::vec3& scale)
    {
        // Check for invertibility
        if (glm::determinant(transform) == 0.0f)
        {
            return false; // Matrix not invertible, decomposition fails
        }

        // Extract translation
        translation = glm::vec3(transform[3]);

        // Extract rotation using SVD
        glm::vec3 s;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(transform, s, rotation, translation, skew, perspective);

        // Normalize rotation to avoid scaling issues
        rotation = glm::normalize(rotation);

        // Check for uniform scaling
        if (std::abs(glm::length(s) - 1.0f) <= glm::epsilon<float>())
        {
            // Uniform scaling detected, adjust rotation
            rotation = glm::quat_cast(glm::transpose(glm::mat3(transform)));
        }

        // Update scale based on extracted values
        scale = s;

        return true;
    }
}
