#pragma once

#include "lights_manager.h"
#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

#include <glm/glm.hpp>

namespace luly::renderer
{
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

    private:
        void generate_poisson_texture();
        void upload_skybox_uniforms() const;

        std::shared_ptr<lights_manager> m_lights_manager;
        std::shared_ptr<shader> m_lighting_shader;
        std::shared_ptr<mesh> m_screen_mesh;
        std::shared_ptr<texture_2d> m_poisson_distribution_texture;
    };
}
