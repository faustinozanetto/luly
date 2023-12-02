#pragma once

#include "renderer/scene/scene_renderer.h"
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
        float get_intensity() const { return m_intensity; }
        const glm::vec3& get_tint() const { return m_tint; }

        /* Setters */
        void set_environment_texture(const std::shared_ptr<renderer::texture_2d>& environment_texture)
        {
            m_environment_texture = environment_texture;
            renderer::scene_renderer::set_environment_map(m_environment_texture);
        }

        void set_intensity(float intensity) { m_intensity = intensity; }
        void set_tint(const glm::vec3& tint) { m_tint = tint; }

    private:
        std::shared_ptr<renderer::texture_2d> m_environment_texture;
        float m_intensity;
        glm::vec3 m_tint;
    };
}
