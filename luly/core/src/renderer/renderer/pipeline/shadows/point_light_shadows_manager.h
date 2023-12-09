#pragma once

#include "renderer/shaders/shader.h"
#include "shadow_manager.h"

#include <memory>

namespace luly::renderer
{
    struct point_light_shadows_data
    {
        float shadow_bias;
    };

    class point_light_shadows_manager : public shadow_manager
    {
    public:
        point_light_shadows_manager();
        ~point_light_shadows_manager() override = default;

        /* Getters */
        point_light_shadows_data& get_point_light_shadows_data()
        {
            return m_point_light_shadows_data;
        }

        /* Overrides */
        void execute(const std::shared_ptr<scene::scene>& current_scene) override;

    private:
        void initialize();
        void initialize_shadows_data();
        void update_shadows_ubo(const std::shared_ptr<point_light>& point_light);
        void calculate_point_light_shadow(const std::shared_ptr<point_light>& point_light);

        std::shared_ptr<shader> m_point_light_shadows_shader;

        std::shared_ptr<uniform_buffer_object> m_point_light_shadows_ubo;
        point_light_shadows_data m_point_light_shadows_data;
    };
}
