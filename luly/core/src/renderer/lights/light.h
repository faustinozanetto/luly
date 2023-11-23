#pragma once

#include <glm/glm.hpp>

namespace luly::renderer
{
    class light
    {
    public:
        light(const glm::vec3& color = glm::vec3(0.85f)) : m_color(color)
        {
        }

        /* Getters */
        const glm::vec3& get_color() const { return m_color; }

        /* Setters */
        void set_color(const glm::vec3& color) { m_color = color; }

    protected:
        glm::vec3 m_color;
    };
}
