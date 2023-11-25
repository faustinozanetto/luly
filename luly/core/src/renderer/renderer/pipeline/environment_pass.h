﻿#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
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

    private:
        void setup_environment();
        void setup_environment_fbo();
        void setup_environment_texture();
        void setup_environment_cubemap();
        void setup_environment_equirectangular_map();

        std::shared_ptr<shader> m_equirectangular_to_cubemap_shader;
        std::shared_ptr<texture_2d> m_environment_map;
        std::shared_ptr<texture_cubemap> m_skybox_texture;
        std::shared_ptr<mesh> m_cube_mesh;

        std::shared_ptr<frame_buffer> m_environment_capture_fbo;
        int m_irradiance_map_size;
        int m_environment_map_size;
        int m_prefilter_map_size;
        int m_brdf_map_size;
    };
}
