#pragma once

#include "renderer/shaders/shader.h"
#include "shadow_manager.h"

#include <memory>

#include "renderer/lights/directional_light.h"

namespace luly::renderer
{
#define CASCADES_COUNT 3

    struct directional_light_shadows_data
    {
        bool show_cascades;
        float shadow_bias;
        float cascade_plane_distances[CASCADES_COUNT];
        glm::vec4 cascade_debug_colors[CASCADES_COUNT];
        bool enable_shadows;
    };

    class directional_light_shadows_manager : public shadow_manager
    {
    public:
        directional_light_shadows_manager();
        ~directional_light_shadows_manager() override = default;

        /* Getters */
        const directional_light_shadows_data& get_directional_light_shadows_data() const
        {
            return m_directional_light_shadows_data;
        }

        /* Setters */
        void set_bias(float bias) { m_directional_light_shadows_data.shadow_bias = bias; }
        void set_show_cascades(bool show_cascades) { m_directional_light_shadows_data.show_cascades = show_cascades; }
        void set_debug_cascade_color(int cascade_index, const glm::vec3& color);

        /* Overrides */
        void execute(const std::shared_ptr<scene::scene>& current_scene) override;
        void bind_uniforms(const std::shared_ptr<shader>& shader) override;

    private:
        void initialize();
        void initialize_shadows_data();
        
        void update_shadows_cascades_data(const std::shared_ptr<directional_light>& directional_light);

        std::shared_ptr<shader> m_directional_light_shadows_shader;

        directional_light_shadows_data m_directional_light_shadows_data;
    };
}
