#include "lypch.h"
#include "animation_controller_component.h"

namespace luly::scene
{
    animation_controller_component::animation_controller_component(
        const std::shared_ptr<renderer::animation_controller>& animation_controller)
    {
        m_animation_controller = animation_controller;
    }

    animation_controller_component::~animation_controller_component()
    {
    }

    void animation_controller_component::initialize()
    {
    }
}
