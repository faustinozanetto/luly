#pragma once

#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

#include <glm/glm.hpp>

#include "renderer/textures/texture_1d.h"

namespace luly::renderer
{
#define MAX_POINT_LIGHTS 6
#define MAX_SPOT_LIGHTS 6

    struct point_light_data
    {
        glm::vec4 color;
        glm::vec4 position;
    };

    struct spot_light_data
    {
        glm::vec4 color;
        glm::vec4 position;
        glm::vec4 direction;
        glm::vec4 angles;
    };

    struct directional_light_data
    {
        glm::vec4 color;
        glm::vec4 direction;
        float shadow_bias;
        float inverse_cascade_factor;
    };

    struct lights_data
    {
        point_light_data point_lights[MAX_POINT_LIGHTS];
        spot_light_data spot_lights[MAX_SPOT_LIGHTS];
        directional_light_data directional_light;
    };

    class lighting_pass : public render_pass
    {
    public:
        lighting_pass();
        ~lighting_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

        void update_lights();

    private:
        void initialize_lights_data();
        void generate_poisson_texture();
        void collect_lights();
        void update_lights_buffer() const;
        void upload_skybox_uniforms();

        lights_data m_lights_data;

        std::shared_ptr<uniform_buffer_object> m_lights_ubo;
        std::shared_ptr<shader> m_lighting_shader;
        std::shared_ptr<mesh> m_screen_mesh;
        std::shared_ptr<texture_2d> m_poisson_distribution_texture;
    };
}
