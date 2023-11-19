#pragma once
#include "panels/ui_panel.h"

namespace luly::ui
{
    class profiling_panel : public ui_panel
    {
    public:
        profiling_panel();
        ~profiling_panel() override;

        /* Overrides */
        void on_render_panel() override;
    };
}
