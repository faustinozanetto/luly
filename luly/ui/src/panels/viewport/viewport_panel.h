#pragma once

#include "panels/ui_panel.h"

namespace luly::ui
{
    class viewport_panel : public ui_panel
    {
    public:
        viewport_panel();
        ~viewport_panel() override;

        /* Overrides */
        void on_render_panel() override;
    };
}
