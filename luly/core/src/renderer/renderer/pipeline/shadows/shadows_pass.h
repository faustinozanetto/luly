#pragma once

#include "directional_light_shadows_manager.h"
#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/lights/point_light.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/textures/texture_2d.h"

#include <memory>

namespace luly::renderer
{
    struct shadows_data
    {
        int soft_shadows;
        int pcf_horizontal_samples;
        int pcf_vertical_samples;
    };

    class shadows_pass : public render_pass
    {
    public:
        shadows_pass();
        ~shadows_pass() override;

        /* Getters */
        const std::shared_ptr<directional_light_shadows_manager>& get_directional_light_shadows_manager() const
        {
            return m_directional_light_shadows_manager;
        }

        shadows_data& get_shadows_data() { return m_shadows_data; }

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

    private:
        void initialize_shadows_data();
        void update_shadows_ubo() const;

        void generate_random_angles_texture();
        /*
                void calculate_point_lights_shadows(const std::shared_ptr<scene::scene>& current_scene) const;
                void calculate_point_light_shadow(const std::shared_ptr<scene::scene>& current_scene,
                                                  const std::shared_ptr<point_light>& point_light) const;
                                                  
        
                std::shared_ptr<shader> m_point_light_shadows_shader;*/

        std::shared_ptr<directional_light_shadows_manager> m_directional_light_shadows_manager;

        /* Cascaded Shadows */
        bool m_show_cascades;
        std::shared_ptr<texture_2d> m_random_angles_texture;

        std::shared_ptr<uniform_buffer_object> m_shadows_ubo;
        shadows_data m_shadows_data;
    };
}
