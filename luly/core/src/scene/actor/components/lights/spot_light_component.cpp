#include "lypch.h"
#include "spot_light_component.h"

namespace luly::scene
{
    spot_light_component::spot_light_component(
        const std::shared_ptr<renderer::spot_light>& spot_light)
    {
        m_spot_light = spot_light;
    }

    spot_light_component::~spot_light_component()
    {
    }

    void spot_light_component::initialize()
    {
    }
}
