#pragma once

#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class geometry_pass : public render_pass
    {
    public:
        geometry_pass();
        ~geometry_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;

    private:
        std::shared_ptr<shader> m_geometry_shader;
    };
}
