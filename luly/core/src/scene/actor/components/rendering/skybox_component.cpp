#include "lypch.h"
#include "skybox_component.h"

#include "renderer/scene/scene_renderer.h"

namespace luly::scene
{
    skybox_component::skybox_component(const std::shared_ptr<renderer::texture_2d>& environment_texture)
    {
        set_environment_texture(environment_texture);
        m_intensity = 1.0f;
    }

    skybox_component::~skybox_component()
    {
    }

    void skybox_component::initialize()
    {
    }
}
