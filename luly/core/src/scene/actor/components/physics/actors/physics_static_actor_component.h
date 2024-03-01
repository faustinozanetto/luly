#pragma once

#include "physics/actors/physics_static_actor.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class physics_static_actor_component : public base_component
    {
    public:
        physics_static_actor_component(const std::shared_ptr<physics::physics_static_actor>& static_actor) :
            m_static_actor(static_actor)
        {
        }

        ~physics_static_actor_component() override
        {
        };

        /* Getters */
        const std::shared_ptr<physics::physics_static_actor>& get_static_actor() const { return m_static_actor; }

        /* Setters */
        void set_static_actor(const std::shared_ptr<physics::physics_static_actor>& static_actor)
        {
            m_static_actor = static_actor;
        }

        void initialize() override
        {
        }

    private:
        std::shared_ptr<physics::physics_static_actor> m_static_actor;
    };
}
