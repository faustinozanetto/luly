#pragma once

#include "panels/ui_panel.h"
#include <ImGuizmo.h>

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
        void render_transform_guizmo() const;
        void render_scene_viewport();
        
        static bool s_show;
        ImGuizmo::OPERATION m_selected_operation;
    };
}
