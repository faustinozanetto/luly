#pragma once
#include "light.h"

namespace luly::renderer
{
    class directional_light : public light
    {
    public:
        directional_light(const glm::vec3& color = glm::vec3(0.85f), const glm::vec3& direction = glm::vec3(0.0f));

        /* Getters */
        const glm::vec3& get_direction() const { return m_direction; }

        /* Setters */
        void set_direction(const glm::vec3& direction) { m_direction = direction; }

    private:
        glm::vec3 m_direction;
    };
}
