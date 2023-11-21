#include "viewport_panel.h"

#include "engine_ui.h"

namespace luly::ui
{
    bool viewport_panel::s_show = true;

    viewport_panel::viewport_panel() : ui_panel("viewport_panel")
    {
    }

    viewport_panel::~viewport_panel()
    {
    }

    void viewport_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Viewport", &s_show))
        {
            // Draw viewport
            const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            const uint32_t render_target = engine_ui::get_ui_data().render_target;
            ImGui::Image(reinterpret_cast<ImTextureID>(render_target),
                         ImVec2{viewportPanelSize.x, viewportPanelSize.y}, ImVec2{0, 1},
                         ImVec2{1, 0});

            ImGui::End();
        }
    }

    bool viewport_panel::get_show_panel()
    {
        return s_show;
    }

    void viewport_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
