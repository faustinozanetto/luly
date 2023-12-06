#pragma once

#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/lights/directional_light.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"
#include "renderer/textures/texture_2d.h"
#include "scene/scene.h"

namespace luly::renderer
{
    struct cascaded_shadows_parameters
    {
        float shadow_bias;
        int soft_shadows;
        int pcf_horizontal_samples;
        int pcf_vertical_samples;
        float cascade_plane_distances[3];
    };

    class shadows_pass : public render_pass
    {
    public:
        shadows_pass();
        ~shadows_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

        /* Getters */
        cascaded_shadows_parameters& get_cascaded_shadows_parameters() { return m_cascaded_shadows_parameters; }

    private:
        void initialize_cascaded_shadows_parameters();
        void generate_random_angles_texture();
        void update_cascaded_shadows_ubo(const std::shared_ptr<directional_light>& directional_light);
        void calculate_directional_light_shadows(const std::shared_ptr<scene::scene>& current_scene,
                                                 const std::shared_ptr<directional_light>& directional_light);

        void render_geometry() const;

        // Cascaded shadows.
        std::shared_ptr<shader> m_directional_light_shadows_shader;
        
        std::shared_ptr<texture_2d> m_random_angles_texture;
        std::shared_ptr<uniform_buffer_object> m_cascaded_shadows_ubo;
        cascaded_shadows_parameters m_cascaded_shadows_parameters;
    };
}
