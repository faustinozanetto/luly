#pragma once

#include "renderer/lights/spot_light.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class spot_light_component : public base_component
    {
    public:
        spot_light_component(const std::shared_ptr<renderer::spot_light>& spot_light);
        ~spot_light_component() override;

        /* Overrides */
        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::spot_light>& get_spot_light() const
        {
            return m_spot_light;
        }

        /* Setters */
        void set_spot_light(const std::shared_ptr<renderer::spot_light>& spot_light)
        {
            m_spot_light = spot_light;
        }

    private:
        std::shared_ptr<renderer::spot_light> m_spot_light;
    };
}
