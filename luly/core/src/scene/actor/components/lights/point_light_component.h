#pragma once

#include "renderer/lights/point_light.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class point_light_component : public base_component
    {
    public:
        point_light_component(const std::shared_ptr<renderer::point_light>& point_light);
        ~point_light_component() override;

        /* Overrides */
        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::point_light>& get_point_light() const
        {
            return m_point_light;
        }

        bool get_enable_shadows() const { return m_enable_shadows; }

        /* Setters */
        void set_point_light(const std::shared_ptr<renderer::point_light>& point_light)
        {
            m_point_light = point_light;
        }

        void set_enable_shadows(bool enable_shadows) { m_enable_shadows = enable_shadows; }

    private:
        std::shared_ptr<renderer::point_light> m_point_light;
        bool m_enable_shadows;
    };
}
