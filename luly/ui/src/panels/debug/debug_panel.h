#pragma once

#include "panels/ui_panel.h"

namespace luly::ui
{
    class debug_panel : public ui_panel
    {
    public:
        debug_panel();
        ~debug_panel() override;

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
