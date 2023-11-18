#include "model.h"

namespace luly::renderer
{
    model::model(const std::vector<std::shared_ptr<mesh>>& meshes)
    {
        m_meshes = meshes;
    }
}
