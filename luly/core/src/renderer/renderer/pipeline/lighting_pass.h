#pragma once

#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

#include <glm/glm.hpp>

namespace luly::renderer
{
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
        float inner_cone_angle;
        float outer_cone_angle;
    };

    struct directional_light_data
    {
        glm::vec4 color;
        glm::vec4 direction;
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

        void update_lights();

    private:
        void initialize_lights_data();
        void collect_lights();
        void update_lights_buffer();

        point_light_data m_point_lights[4];
        spot_light_data m_spot_lights[4];
        directional_light_data m_directional_light;

        std::shared_ptr<uniform_buffer_object> m_lights_ubo;
        std::shared_ptr<shader> m_lighting_shader;
        std::shared_ptr<mesh> m_screen_mesh;
    };
}
