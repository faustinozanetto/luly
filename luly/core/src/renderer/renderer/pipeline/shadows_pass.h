#pragma once

#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class shadows_pass : public render_pass
    {
    public:
        shadows_pass();
        ~shadows_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;

    private:
        void calculate_directional_light_shadows();

        void render_geometry();
        std::shared_ptr<shader> m_directional_light_shadows_shader;
    };
}
