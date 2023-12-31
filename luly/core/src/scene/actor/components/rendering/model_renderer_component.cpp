﻿#include "lypch.h"
#include "model_renderer_component.h"

namespace luly::scene
{
    model_renderer_component::model_renderer_component(const std::shared_ptr<renderer::model>& model)
    {
        m_model = model;
        m_casts_shadows = true;
    }

    model_renderer_component::~model_renderer_component()
    {
    }

    void model_renderer_component::initialize()
    {
    }
}
