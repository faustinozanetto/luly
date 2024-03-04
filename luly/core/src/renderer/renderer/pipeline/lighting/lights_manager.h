#pragma once

#include "renderer/buffers/uniform/uniform_buffer_object.h"

#include <glm/glm.hpp>
#include <memory>

namespace luly::renderer
{
#define MAX_POINT_LIGHTS 6
#define MAX_SPOT_LIGHTS 6

    struct point_light_data
    {
        glm::vec4 color;
        glm::vec4 position;
        glm::vec4 parameters; // intensity, constant-factor, linear-factor, quadratic-factor
    };

    struct spot_light_data
    {
        glm::vec4 color;
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec4 parameters; // angels, intensity, unused, unused
    };

    struct directional_light_data
    {
        glm::vec4 color;
        glm::vec4 direction;
        glm::vec4 parameters; // intensity, unused, unused, unused
    };

    struct lights_data
    {
        glm::ivec4 parameters; // point light count, spot light count, unused , unused.
    };

    class lights_manager
    {
    public:
        lights_manager();

        /* Methods */
        void update_lights();

    private:
        void initialize_data();
        void collect_lights();
        void update_lights_buffer() const;

        point_light_data m_point_lights[MAX_POINT_LIGHTS];
        spot_light_data m_spot_lights[MAX_SPOT_LIGHTS];
        directional_light_data m_directional_light;
        lights_data m_lights;

        std::shared_ptr<uniform_buffer_object> m_lights_ubo;
    };
}
