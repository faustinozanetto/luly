#include "lypch.h"
#include "point_light_component.h"

namespace luly::scene
{
    point_light_component::point_light_component(
        const std::shared_ptr<renderer::point_light>& point_light)
    {
        m_point_light = point_light;
        m_enable_shadows = true;
    }

    point_light_component::~point_light_component()
    {
    }

    void point_light_component::initialize()
    {
    }
}
