#pragma once
#include "panels/ui_panel.h"

namespace luly::ui
{
    class scene_hierarchy_panel : public ui_panel
    {
    public:
        scene_hierarchy_panel();
        ~scene_hierarchy_panel() override;

        /* Overrides */
        void on_render_panel() override;
    };
}
