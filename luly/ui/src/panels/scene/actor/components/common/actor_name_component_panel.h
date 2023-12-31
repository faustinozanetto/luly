﻿#pragma once

#include "panels/scene/actor/components/actor_component_panel.h"

namespace luly::ui
{
    class actor_name_component_panel : public actor_component_panel
    {
    public:
        actor_name_component_panel();

        /* Overrides */
        std::pair<bool, size_t> get_actor_component_details() override;
        void on_render_component_details() override;
    };
}
