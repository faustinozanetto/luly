#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    struct bloom_pass_mip_data
    {
        glm::ivec2 size;
        std::shared_ptr<texture_2d> texture;
    };

    class bloom_pass : public render_pass
    {
    public:
        bloom_pass();
        ~bloom_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

        /* Getters */
        const std::vector<bloom_pass_mip_data>& get_mips_data() const { return m_mips; }
        float get_filter_radius() const { return m_filter_radius; }
        float get_strength() const { return m_strength; }

        /* Setters */
        void set_filter_radius(float filter_radius) { m_filter_radius = filter_radius; }
        void set_strength(float strength) { m_strength = strength; }

    private:
        void generate_fbo();
        void generate_mips();
        void attach_mip_texture();
        
        void perform_down_sample();
        void perform_up_sample();
        void perform_composition();

        std::shared_ptr<shader> m_down_sample_shader;
        std::shared_ptr<shader> m_up_sample_shader;
        std::shared_ptr<shader> m_composition_shader;

        std::shared_ptr<mesh> m_screen_mesh;
        std::vector<bloom_pass_mip_data> m_mips;
        int m_samples_count;
        float m_filter_radius;
        float m_strength;
    };
}
