#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class final_pass : public render_pass
    {
    public:
        final_pass();
        ~final_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;

    private:
        std::shared_ptr<shader> m_screen_shader;
        std::shared_ptr<mesh> m_screen_mesh;
    };
}
