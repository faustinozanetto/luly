﻿#pragma once

#include "meshes/mesh.h"

#include <vector>
#include <memory>

namespace luly::renderer
{
    class model
    {
    public:
        model(const std::vector<std::shared_ptr<mesh>>& meshes);

        /* Getters */
        const std::vector<std::shared_ptr<mesh>>& get_meshes() const { return m_meshes; }

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
    };
}
