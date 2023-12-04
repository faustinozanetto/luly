#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class tonemapping_pass : public render_pass
    {
    public:
        tonemapping_pass();
        ~tonemapping_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

        /* Getters */
        float get_exposure() const { return m_exposure; }
        float get_gamma() const { return m_gamma; }

        /* Setters */
        void set_exposure(float exposure) { m_exposure = exposure; }
        void set_gamma(float gamma) { m_gamma = gamma; }

    private:
        std::shared_ptr<shader> m_tonemapping_shader;
        std::shared_ptr<mesh> m_screen_mesh;

        float m_exposure;
        float m_gamma;
    };
}
