#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class debanding_pass : public render_pass
    {
    public:
        debanding_pass();
        ~debanding_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

    private:
        std::shared_ptr<shader> m_debanding_shader;
        std::shared_ptr<mesh> m_screen_mesh;

        float m_intensity;
        float m_threshold;
    };
}
