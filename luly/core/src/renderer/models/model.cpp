#include "lypch.h"
#include "model.h"

namespace luly::renderer
{
    model::model(const std::vector<std::shared_ptr<mesh>>& meshes,
                 const std::unordered_map<std::string, std::shared_ptr<material>>& materials)
    {
        m_meshes = meshes;
        m_materials = materials;
    }
}
