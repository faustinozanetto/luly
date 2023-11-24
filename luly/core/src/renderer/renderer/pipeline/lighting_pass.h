#pragma once

#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    struct point_light_data
    {
        glm::vec4 color;
        glm::vec4 position;
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

        void update_lights();

    private:
        void collect_lights();
        void update_lights_buffer();

        directional_light_data m_directional_light;
        point_light_data m_point_lights[10];
        std::shared_ptr<uniform_buffer_object> m_lights_ubo;
        std::shared_ptr<shader> m_lighting_shader;
        std::shared_ptr<mesh> m_screen_mesh;
    };
}
