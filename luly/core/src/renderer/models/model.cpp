#include "lypch.h"
#include "model.h"

namespace luly::renderer
{
    model::model(const std::vector<std::shared_ptr<mesh>>& meshes,  const std::vector<std::shared_ptr<material>>& materials)
    {
        m_meshes = meshes;
        m_materials = materials;
    }
}
