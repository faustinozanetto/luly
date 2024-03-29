﻿#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/renderer/pass/render_pass.h"
#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class antialiasing_pass : public render_pass
    {
    public:
        antialiasing_pass();
        ~antialiasing_pass() override;

        /* Overrides */
        void initialize() override;
        void execute() override;
        void set_outputs() override;
        void on_resize(const glm::ivec2& dimensions) override;

    private:
        std::shared_ptr<shader> m_antialiasing_shader;
        std::shared_ptr<mesh> m_screen_mesh;
    };
}
