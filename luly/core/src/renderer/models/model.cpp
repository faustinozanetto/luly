#include "lypch.h"
#include "model.h"

namespace luly::renderer
{
    model::model(const std::vector<std::shared_ptr<mesh>>& meshes,
                 const std::unordered_map<std::string, std::shared_ptr<material>>& materials,
                 const std::map<std::string, model_bone_info>& bones_info, int bones_count)
    {
        m_meshes = meshes;
        m_materials = materials;
        m_bones_count = bones_count;
        m_bones_info = bones_info;
    }

    model::~model()
    {
    }
}
