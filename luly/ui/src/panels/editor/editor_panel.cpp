#include "editor_panel.h"

#include "engine_ui.h"

#include "utils/ui_utils.h"

namespace luly::ui
{
    bool editor_panel::s_show = true;

    editor_panel::editor_panel() : ui_panel("editor_panel")
    {
    }

    editor_panel::~editor_panel()
    {
    }

    void editor_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Editor", &s_show))
        {
            ui_utils::draw_property("Show Guizmos", engine_ui::get_ui_data().show_guizmos);
            if (ui_utils::draw_property("Guizmos Scale", engine_ui::get_ui_data().guizmos_scale, 0.0f, 2.0f, 0.01f))
            {
                ImGuizmo::SetGizmoSizeClipSpace(engine_ui::get_ui_data().guizmos_scale / 2.0f);
            }
            ui_utils::draw_property("Snap Value", engine_ui::get_ui_data().snap_value, 0.0f, 10.0f, 0.05f);
            ImGui::End();
        }
    }

    bool editor_panel::get_show_panel()
    {
        return s_show;
    }

    void editor_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
