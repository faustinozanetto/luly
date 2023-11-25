#pragma once

#include "renderer/textures/texture_2d.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class skybox_component : public base_component
    {
    public:
        skybox_component(const std::shared_ptr<renderer::texture_2d>& environment_texture);
        ~skybox_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::texture_2d>& get_environment_texture() const { return m_environment_texture; }

        /* Setters */
        void set_environment_texture(const std::shared_ptr<renderer::texture_2d>& environment_texture)
        {
            m_environment_texture = environment_texture;
        }

    private:
        std::shared_ptr<renderer::texture_2d> m_environment_texture;;
    };
}
