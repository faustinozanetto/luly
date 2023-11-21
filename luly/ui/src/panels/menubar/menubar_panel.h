#pragma once

#include "panels/ui_panel.h"

namespace luly::ui
{
    class LULY_UI_API menubar_panel : public ui_panel
    {
    public:
        menubar_panel();
        ~menubar_panel() override;

        /* Overrides*/
        void on_render_panel() override;
        bool get_show_panel() override { return true; }
        void set_show_panel(bool show_panel) override {}

    private:
        bool m_show_styles_editor;
    };
}
