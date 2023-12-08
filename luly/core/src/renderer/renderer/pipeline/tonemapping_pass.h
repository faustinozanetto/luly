#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    enum class tonemapping_type
    {
        linear = 0,
        reinhard,
        aces,
        filmic,
        hable,
        luma
    };

    static const tonemapping_type TONEMAPPING_TYPES[] = {tonemapping_type::linear, tonemapping_type::reinhard, tonemapping_type::aces,
		tonemapping_type::filmic, tonemapping_type::hable, tonemapping_type::luma };

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
        tonemapping_type get_type() const { return m_type; }

        /* Setters */
        void set_exposure(float exposure) { m_exposure = exposure; }
        void set_gamma(float gamma) { m_gamma = gamma; }
        void set_type(tonemapping_type type) { m_type = type; }

        /* Utils */
        static const char* get_tonemapping_type_to_string(tonemapping_type type);

    private:
        std::shared_ptr<shader> m_tonemapping_shader;
        std::shared_ptr<mesh> m_screen_mesh;

        float m_exposure;
        float m_gamma;
        tonemapping_type m_type;
    };
}
