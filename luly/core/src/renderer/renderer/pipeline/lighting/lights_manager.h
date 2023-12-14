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
        glm::vec3 color;
        float intensity;
        //glm::vec2 pad1; // Padding to align the structure to 16 bytes
        glm::vec3 position;
        float constant_factor;
        float linear_factor;
        float quadratic_factor;
    };

    struct spot_light_data
    {
        glm::vec3 color;
        float intensity;
      //  glm::vec2 pad1; // Padding to align the structure to 16 bytes
        glm::vec3 position;
       // glm::vec2 pad2; // Padding to align the structure to 16 bytes
        glm::vec3 direction;
       // glm::vec2 pad3; // Padding to align the structure to 16 bytes
        glm::vec2 angles;
    };

    struct directional_light_data
    {
        glm::vec3 color;
        float intensity;
        //glm::vec2 pad1; // Padding to align the structure to 16 bytes
        glm::vec3 direction;
    };

    struct lights_data
    {
        point_light_data point_lights[MAX_POINT_LIGHTS];
        spot_light_data spot_lights[MAX_SPOT_LIGHTS];
        directional_light_data directional_light;
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

        lights_data m_lights_data;
        std::shared_ptr<uniform_buffer_object> m_lights_ubo;
    };
}
