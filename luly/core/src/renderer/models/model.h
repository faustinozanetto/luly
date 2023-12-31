﻿#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/materials/material.h"

#include <vector>
#include <memory>

namespace luly::renderer
{
    class model
    {
    public:
        model(const std::vector<std::shared_ptr<mesh>>& meshes,
              const std::vector<std::shared_ptr<material>>& materials = {});

        /* Getters */
        const std::vector<std::shared_ptr<mesh>>& get_meshes() const { return m_meshes; }
        const std::vector<std::shared_ptr<material>>& get_materials() const { return m_materials; }

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
        std::vector<std::shared_ptr<material>> m_materials;
    };
}
