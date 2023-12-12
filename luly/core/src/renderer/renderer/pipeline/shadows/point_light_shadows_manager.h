#pragma once

#include "shadow_manager.h"
#include "renderer/shaders/shader.h"
#include "renderer/lights/point_light.h"
#include "renderer/renderer/pipeline/lighting_pass.h"
#include "renderer/renderer/renderer.h"

#include <memory>

namespace luly::renderer
{
    struct point_light_shadows_data
    {
        float shadow_bias;
        float far_planes[MAX_POINT_LIGHTS];
        bool enable_shadows[MAX_POINT_LIGHTS];
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
        void bind_uniforms(const std::shared_ptr<shader>& shader) override;

    private:
        void initialize();
        void initialize_shadows_data();
        void calculate_point_light_shadow(const std::shared_ptr<point_light>& point_light);

        std::shared_ptr<shader> m_point_light_shadows_shader;

        point_light_shadows_data m_point_light_shadows_data;
    };
}
