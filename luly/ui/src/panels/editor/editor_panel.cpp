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
            bool show_guizmos = engine_ui::get().get_show_guizmos();
            if (ui_utils::draw_property("Show Guizmos", show_guizmos))
            {
                engine_ui::get().set_show_guizmos(show_guizmos);
            }

            float guizmos_scale = engine_ui::get().get_guizmos_scale();
            if (ui_utils::draw_property("Guizmos Scale", guizmos_scale, 0.0f, 2.0f, 0.01f))
            {
                engine_ui::get().set_guizmos_scale(guizmos_scale);
                ImGuizmo::SetGizmoSizeClipSpace(guizmos_scale / 2.0f);
            }

            float snap_value = engine_ui::get().get_snap_value();
            if (ui_utils::draw_property("Snap Value", snap_value, 0.0f, 10.0f, 0.05f))
            {
                engine_ui::get().set_snap_value(snap_value);
            }
        }
        ImGui::End();
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
