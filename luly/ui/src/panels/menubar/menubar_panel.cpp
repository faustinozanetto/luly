#include "menubar_panel.h"

#include "engine_ui.h"

namespace luly::ui
{
    menubar_panel::menubar_panel() : ui_panel("menubar_panel")
    {
        m_show_styles_editor = false;
    }

    menubar_panel::~menubar_panel()
    {
    }

    void menubar_panel::on_render_panel()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("Test");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Settings"))
            {
                m_show_styles_editor = ImGui::MenuItem("Styles Editor");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Panels"))
            {
                for (const auto& ui_panel : engine_ui::get_ui_data().panels)
                {
                    // Skip this panel.
                    if (ui_panel->get_name() == "menubar_panel") continue;

                    if (ImGui::MenuItem(ui_panel->get_name().c_str(), "", ui_panel->get_show_panel(), true))
                    {
                        ui_panel->set_show_panel(!ui_panel->get_show_panel());
                    }
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (m_show_styles_editor)
        {
            ImGui::Begin("Dear ImGui Style Editor", &m_show_styles_editor);
            ImGui::ShowStyleEditor();
            ImGui::End();
        }
    }
}
