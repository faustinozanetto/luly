﻿#pragma once

#include "base_component.h"
#include "math/transform/transform.h"

namespace luly::scene
{
    class transform_component : public base_component
    {
    public:
        transform_component(const glm::vec3& location = glm::vec3(0.0f),
                            const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0, 0.0f),
                            const glm::vec3& scale = glm::vec3(1.0f));
        transform_component(const std::shared_ptr<math::transform>& transform);
        ~transform_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<math::transform>& get_transform() const { return m_transform; }

        /* Setters */
        void set_transform(const std::shared_ptr<math::transform>& transform) { m_transform = transform; }

    private:
        std::shared_ptr<math::transform> m_transform;
    };
}
