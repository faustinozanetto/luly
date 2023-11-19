#include "viewport_panel.h"

#include "engine_ui.h"

namespace luly::ui
{
    viewport_panel::viewport_panel() : ui_panel("viewport_panel")
    {
    }

    viewport_panel::~viewport_panel()
    {
    }

    void viewport_panel::on_render_panel()
    {
        ImGui::Begin("Viewport", &m_show);

        // Draw viewport
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        const uint32_t render_target = engine_ui::get_ui_data().render_target;
        ImGui::Image(reinterpret_cast<ImTextureID>(render_target),
                     ImVec2{viewportPanelSize.x, viewportPanelSize.y}, ImVec2{0, 1},
                     ImVec2{1, 0});

        ImGui::End();
    }
}
