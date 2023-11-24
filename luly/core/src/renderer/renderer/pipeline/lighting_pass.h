#pragma once

#include "renderer/buffers/uniform/uniform_buffer_object.h"
#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    struct directional_light_data
    {
        glm::vec4 color;
        glm::vec4 direction;
    };

    struct lights_data
    {
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

        void update_lights();

    private:
         void collect_lights();
         void update_lights_buffer();

        lights_data m_lights_data;
        std::shared_ptr<uniform_buffer_object> m_lights_ubo;
        std::shared_ptr<shader> m_lighting_shader;
        std::shared_ptr<mesh> m_screen_mesh;
    };
}
