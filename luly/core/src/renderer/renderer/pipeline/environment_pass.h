#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/render_buffer/render_buffer.h"
#include "renderer/shaders/shader.h"
#include "renderer/textures/texture_2d.h"
#include "renderer/textures/texture_cubemap.h"

namespace luly::renderer
{
    class environment_pass : public render_pass
    {
    public:
        environment_pass();
        ~environment_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
    void set_outputs() override;
        
        /* Getters */
        const std::shared_ptr<texture_cubemap>& get_irradiance_map() const { return m_environment_irradiance_texture; }

        /* Methods */
        void set_environment_map(const std::shared_ptr<texture_2d>& environment_map);

    private:
        void setup_environment();
        void setup_environment_fbo();
        void setup_environment_cubemap();
        void setup_environment_equirectangular_map();
        void setup_irradiance_map();
        void setup_prefilter_map();
        void setup_brdf_map();

        glm::mat4 m_capture_projection;
        std::vector<glm::mat4> m_capture_views;

        std::shared_ptr<shader> m_equirectangular_to_cubemap_shader;
        std::shared_ptr<shader> m_irradiance_shader;
        std::shared_ptr<shader> m_prefilter_shader;
        std::shared_ptr<shader> m_brdf_shader;

        std::shared_ptr<texture_2d> m_environment_hdr_texture;
        std::shared_ptr<texture_2d> m_brdf_texture;
        std::shared_ptr<texture_cubemap> m_environment_irradiance_texture;
        std::shared_ptr<texture_cubemap> m_environment_cubemap_texture;
        std::shared_ptr<texture_cubemap> m_environment_prefilter_texture;

        std::shared_ptr<mesh> m_cube_mesh;
        std::shared_ptr<mesh> m_quad_mesh;

        std::shared_ptr<frame_buffer> m_environment_capture_fbo;
        std::shared_ptr<render_buffer> m_environment_capture_rbo;
        int m_irradiance_map_size;
        int m_environment_map_size;
        int m_prefilter_map_size;
        int m_brdf_map_size;
    };
}
