#include "menubar_panel.h"

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
