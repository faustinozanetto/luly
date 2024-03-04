#include "lypch.h"
#include "math_utils.h"

#include <glm/gtx/matrix_decompose.hpp>

namespace luly::math
{
    bool math_utils::decompose_transform(const glm::mat4& transform, glm::vec3& translation, glm::quat& rotation,
                                         glm::vec3& scale)
    {
        LY_PROFILE_FUNCTION;
        // Check for invertibility
        if (determinant(transform) == 0.0f)
        {
            return false; // Matrix not invertible, decomposition fails
        }

        // Extract translation
        translation = glm::vec3(transform[3]);

        // Extract rotation using SVD
        glm::vec3 s;
        glm::vec3 skew;
        glm::vec4 perspective;
        decompose(transform, s, rotation, translation, skew, perspective);

        // Normalize rotation to avoid scaling issues
        rotation = normalize(rotation);

        // Check for uniform scaling
        if (std::abs(length(s) - 1.0f) <= glm::epsilon<float>())
        {
            // Uniform scaling detected, adjust rotation
            rotation = quat_cast(transpose(glm::mat3(transform)));
        }

        // Update scale based on extracted values
        scale = s;

        return true;
    }
}
