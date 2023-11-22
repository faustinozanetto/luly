#include "lypch.h"
#include "material_component.h"

namespace luly::scene
{
    material_component::material_component(const std::shared_ptr<renderer::material>& material)
    {
        m_material = material;
    }

    material_component::~material_component()
    {
    }

    void material_component::initialize()
    {
    }
}
