#pragma once

#include "panels/ui_panel.h"

namespace luly::ui
{
    class menubar_panel : public ui_panel
    {
    public:
        menubar_panel();
        ~menubar_panel() override;

        /* Overrides*/
        void on_render_panel() override;

    private:
        bool m_show_styles_editor;
    };
}
