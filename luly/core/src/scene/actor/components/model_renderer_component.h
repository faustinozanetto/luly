#pragma once

#include "base_component.h"

#include <models/model.h>

namespace luly::scene
{
    class model_renderer_component : public base_component
    {
    public:
        model_renderer_component();
        model_renderer_component(const std::shared_ptr<renderer::model>& model);
        ~model_renderer_component() override;

        void initialize() override;

        /* Getters */
        const std::shared_ptr<renderer::model>& get_model() const { return m_model; }

        /* Setters */
        void set_model(const std::shared_ptr<renderer::model>& model) { m_model = model; }

    private:
        std::shared_ptr<renderer::model> m_model;
    };
}
