#pragma once

#include "panels/ui_panel.h"

#include <core_api.h>

namespace luly::ui
{
    class LULY_UI_API profiling_panel : public ui_panel
    {
    public:
        profiling_panel();
        ~profiling_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        static bool s_show;
    };
}
