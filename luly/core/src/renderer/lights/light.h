#pragma once

#include <glm/glm.hpp>

namespace luly::renderer
{
    class light
    {
    public:
        light(const glm::vec3& color = glm::vec3(0.85f), float instensity = 1.0f) : m_color(color),
            m_intensity(instensity)
        {
        }

        /* Getters */
        const glm::vec3& get_color() const { return m_color; }
        float get_intensity() const { return m_intensity; }

        /* Setters */
        void set_color(const glm::vec3& color) { m_color = color; }
        void set_intensity(float intensity) { m_intensity = intensity; }

    protected:
        glm::vec3 m_color;
        float m_intensity;
    };
}
