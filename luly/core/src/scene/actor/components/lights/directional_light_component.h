#pragma once

#include "renderer/lights/directional_light.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class directional_light_component : public base_component
    {
    public:
        directional_light_component(const std::shared_ptr<renderer::directional_light>& directional_light);
        ~directional_light_component() override;

        /* Overrides */
        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::directional_light>& get_directional_light() const
        {
            return m_directional_light;
        }

        /* Setters */
        void set_directional_light(const std::shared_ptr<renderer::directional_light>& directional_light)
        {
            m_directional_light = directional_light;
        }

    private:
        std::shared_ptr<renderer::directional_light> m_directional_light;
    };
}
