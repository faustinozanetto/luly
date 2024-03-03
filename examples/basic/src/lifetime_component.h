#pragma once

#include "scene/actor/components/base_component.h"

class lifetime_component : public luly::scene::base_component
{
public:
    lifetime_component(float lifetime);

    void initialize() override;
    void on_update(float delta_time);

private:
    float m_lifetime;
};
