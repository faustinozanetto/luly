#pragma once

#include "scene/actor/components/base_component.h"

#include "renderer/models/model.h"

namespace luly::scene
{
    class model_renderer_component : public base_component
    {
    public:
        model_renderer_component(const std::shared_ptr<renderer::model>& model);
        ~model_renderer_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::model>& get_model() const { return m_model; }
        bool get_casts_shadows() const { return m_casts_shadows; }

        /* Setters */
        void set_model(const std::shared_ptr<renderer::model>& model) { m_model = model; }
        void set_casts_shadows(bool casts_shadows) { m_casts_shadows = casts_shadows; }

    private:
        std::shared_ptr<renderer::model> m_model;
        bool m_casts_shadows;
    };
}
