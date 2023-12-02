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

    private:
        std::shared_ptr<shader> m_down_sample_shader;
        std::shared_ptr<shader> m_up_sample_shader;
        std::shared_ptr<shader> m_composition_shader;

        std::shared_ptr<mesh> m_screen_mesh;
        std::vector<bloom_pass_mip_data> m_mips;
        int m_samples_count;
    };
}
