#include "lypch.h"
#include "directional_light_component.h"

namespace luly::scene
{
    directional_light_component::directional_light_component(
        const std::shared_ptr<renderer::directional_light>& directional_light)
    {
        m_directional_light = directional_light;
        m_enable_shadows = true;
    }

    directional_light_component::~directional_light_component()
    {
    }

    void directional_light_component::initialize()
    {
    }
}
