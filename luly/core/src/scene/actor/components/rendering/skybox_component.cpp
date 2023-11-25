#include "lypch.h"
#include "skybox_component.h"

#include "renderer/scene/scene_renderer.h"

namespace luly::scene
{
    skybox_component::skybox_component(const std::shared_ptr<renderer::texture_2d>& environment_texture)
    {
        m_environment_texture = environment_texture;

        renderer::scene_renderer::set_environment_map(m_environment_texture);
    }

    skybox_component::~skybox_component()
    {
    }

    void skybox_component::initialize()
    {
    }
}
