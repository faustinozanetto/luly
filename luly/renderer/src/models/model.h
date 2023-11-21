#pragma once

#include "meshes/mesh.h"
#include "renderer_api.h"

#include <vector>
#include <memory>

namespace luly::renderer
{
    class LULY_RENDERER_API model
    {
    public:
        model(const std::vector<std::shared_ptr<mesh>>& meshes);

        /* Getters */
        const std::vector<std::shared_ptr<mesh>>& get_meshes() const { return m_meshes; }

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
    };
}
