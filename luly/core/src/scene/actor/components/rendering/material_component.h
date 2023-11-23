#pragma once

#include "renderer/materials/material.h"
#include "scene/actor/components/base_component.h"

namespace luly::scene
{
    class material_component : public base_component
    {
    public:
        material_component(const std::shared_ptr<renderer::material>& material);
        ~material_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::material>& get_material() const { return m_material; }

        /* Setters */
        void set_material(const std::shared_ptr<renderer::material>& material) { m_material = material; }

    private:
        std::shared_ptr<renderer::material> m_material;
    };
}
