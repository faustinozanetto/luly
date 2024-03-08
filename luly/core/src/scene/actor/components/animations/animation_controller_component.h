#pragma once

#include "renderer/animations/animation_controller.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class animation_controller_component : public base_component
    {
    public:
        animation_controller_component(const std::shared_ptr<renderer::animation_controller>& animation_controller);
        ~animation_controller_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::animation_controller>& get_animation_controller() const
        {
            return m_animation_controller;
        }

        /* Setters */
        void set_animation_controller(const std::shared_ptr<renderer::animation_controller>& animation_controller)
        {
            m_animation_controller = animation_controller;
        }

    private:
        std::shared_ptr<renderer::animation_controller> m_animation_controller;
    };
}
