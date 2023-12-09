#pragma once

#include "renderer/shaders/shader.h"
#include "shadows_pass.h"

#include <memory>

namespace luly::renderer
{
    struct directional_light_shadows_data
    {
        int show_cascades;
        float shadow_bias;
        float cascade_plane_distances[CASCADES_COUNT];
        glm::vec4 cascade_debug_colors[CASCADES_COUNT];
    };

    class directional_light_shadows_manager
    {
    public:
        directional_light_shadows_manager();
        ~directional_light_shadows_manager();

        /* Setters */
        void set_show_cascades(bool show_cascades) { m_directional_light_shadows_data.shadow_bias = show_cascades; }
        void set_debug_cascade_color(int cascade_index, const glm::vec3& color);

    private:
        void initialize();
        void initialize_shadows_data();
        
        std::shared_ptr<shader> m_directional_light_shadows_shader;

        std::shared_ptr<uniform_buffer_object> m_directional_light_shadows_ubo;
        directional_light_shadows_data m_directional_light_shadows_data;
    };
}
