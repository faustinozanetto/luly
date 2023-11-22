﻿#pragma once

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
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        static bool s_show;
    };
}
