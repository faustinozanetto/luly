#pragma once

#include "light.h"

namespace luly::renderer
{
    class point_light : public light
    {
    public:
        point_light(const glm::vec3& color = glm::vec3(0.8f), const glm::vec3& position = glm::vec3(0.0f));

        /* Getters */
        const glm::vec3& get_position() const { return m_position; }

        /* Setters */
        void set_position(const glm::vec3& position) { m_position = position; }

    private:
        glm::vec3 m_position;
    };
}
