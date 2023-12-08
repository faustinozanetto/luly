#pragma once

#include "panels/ui_panel.h"

namespace luly::ui
{
    class editor_panel : public ui_panel
    {
    public:
        editor_panel();
        ~editor_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        void render_transform_guizmo() const;
        void render_toolbar();
        void render_scene_viewport();

        static bool s_show;
    };
}
