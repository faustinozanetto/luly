#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"
#include "renderer/textures/texture_2d.h"

namespace luly::renderer
{
    class ambient_occlusion_pass : public render_pass
    {
    public:
        ambient_occlusion_pass();
        ~ambient_occlusion_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;

        /* Getters */
        float get_ssao_radius() const { return m_ssao_radius; }
        float get_ssao_bias() const { return m_ssao_bias; }
        float get_ssao_noise_size() const { return m_ssao_noise_size; }

        /* Setters */
        void set_ssao_radius(float ssao_radius) { m_ssao_radius = ssao_radius; }
        void set_ssao_bias(float ssao_bias) { m_ssao_bias = ssao_bias; }
        void set_ssao_noise_size(float ssao_noise_size) { m_ssao_noise_size = ssao_noise_size; }

    private:
        std::shared_ptr<shader> m_ssao_shader;
        std::shared_ptr<shader> m_blur_shader;
        std::shared_ptr<mesh> m_screen_mesh;
        std::shared_ptr<frame_buffer> m_blur_fbo;

        std::vector<glm::vec3> m_ssao_kernel;
        std::vector<glm::vec3> m_ssao_noise;
        std::shared_ptr<texture_2d> m_ssao_noise_texture;
        float m_ssao_radius;
        float m_ssao_bias;
        float m_ssao_noise_size;
        int m_ssao_kernel_size;
        int m_ssao_noise_sample_size;
    };
}
