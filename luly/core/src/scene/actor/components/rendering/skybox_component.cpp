#include "lypch.h"
#include "skybox_component.h"

namespace luly::scene
{
    skybox_component::skybox_component(const std::shared_ptr<renderer::texture>& environment_texture)
    {
        m_environment_texture = environment_texture;
    }

    skybox_component::~skybox_component()
    {
    }

    void skybox_component::initialize()
    {
    }
}
