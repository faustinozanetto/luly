﻿#pragma once

#include "scene/actor/components/base_component.h"
#include "physics/physics_material.h"

namespace luly::scene
{
    class physics_material_component : public base_component
    {
    public:
        physics_material_component()
        {
        }

        physics_material_component(
            const std::shared_ptr<physics::physics_material>& physics_material) : m_physics_material(physics_material)
        {
        }

        ~physics_material_component() override
        {
        }

        void initialize() override
        {
            if (!m_physics_material)
                m_physics_material = std::make_shared<physics::physics_material>();
        }

        /* Getters */
        const std::shared_ptr<physics::physics_material>& get_physics_material() const { return m_physics_material; }

        /* Setters */
        void set_physics_material(const std::shared_ptr<physics::physics_material>& physics_material)
        {
            m_physics_material = physics_material;
        }

    private:
        std::shared_ptr<physics::physics_material> m_physics_material;
    };
}
