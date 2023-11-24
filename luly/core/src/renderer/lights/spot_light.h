#pragma once

#include "light.h"

namespace luly::renderer
{
    class spot_light : public light
    {
    public:
        spot_light(const glm::vec3& color = glm::vec3(0.8f), const glm::vec3& position = glm::vec3(0.0f),
                   const glm::vec3& direction = glm::vec3(0.0f),
                   float inner_cone_angle = 15.0f, float outer_cone_angle = 50.0f);

        /* Getters */
        const glm::vec3& get_position() const { return m_position; }
        const glm::vec3& get_direction() const { return m_direction; }
        float get_inner_cone_angle() const { return m_inner_cone_angle; }
        float get_outer_cone_angle() const { return m_outer_cone_angle; }

        /* Setters */
        void set_position(const glm::vec3& position) { m_position = position; }
        void set_direction(const glm::vec3& direction) { m_direction = direction; }
        void set_inner_cone_angle(float inner_cone_angle) { m_inner_cone_angle = inner_cone_angle; }
        void set_outer_cone_angle(float outer_cone_angle) { m_outer_cone_angle = outer_cone_angle; }

    private:
        glm::vec3 m_position;
        glm::vec3 m_direction;
        float m_inner_cone_angle;
        float m_outer_cone_angle;
    };
}
