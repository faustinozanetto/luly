#pragma once

#include "renderer/shaders/shader.h"
#include "shadow_manager.h"

#include <memory>

namespace luly::renderer
{
#define CASCADES_COUNT 3

    struct directional_light_shadows_data
    {
        bool show_cascades;
        float shadow_bias;
        float cascade_plane_distances[CASCADES_COUNT];
        glm::vec4 cascade_debug_colors[CASCADES_COUNT];
    };

    class directional_light_shadows_manager : public shadow_manager
    {
    public:
        directional_light_shadows_manager();
        ~directional_light_shadows_manager() override = default;

        /* Getters */
        directional_light_shadows_data& get_directional_light_shadows_data()
        {
            return m_directional_light_shadows_data;
        }

        /* Setters */
        void set_debug_cascade_color(int cascade_index, const glm::vec3& color);

        /* Overrides */
        void execute(const std::shared_ptr<scene::scene>& current_scene) override;

    private:
        void initialize();
        void initialize_shadows_data();
        void update_shadows_ubo(const std::shared_ptr<directional_light>& directional_light);

        std::shared_ptr<shader> m_directional_light_shadows_shader;

        std::shared_ptr<uniform_buffer_object> m_directional_light_shadows_ubo;
        directional_light_shadows_data m_directional_light_shadows_data;
    };
}
