#include "menubar_panel.h"

#include "scene/serialization/scene_serializer.h"
#include "engine_ui.h"
#include "scene/scene_manager.h"

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
            if (ImGui::BeginMenu("Scene"))
            {
                if (ImGui::MenuItem("Save to File"))
                {
                    const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().
                        get_current_scene();
                    if (current_scene)
                    {
                        const std::shared_ptr<scene::scene_serializer>& scene_serializer = std::make_shared<
                            scene::scene_serializer>(current_scene);
                        scene_serializer->serialize("assets/scenes/" + current_scene->get_name() + ".yaml");
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Settings"))
            {
                m_show_styles_editor = ImGui::MenuItem("Styles Editor");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Panels"))
            {
                for (const std::shared_ptr<ui_panel>& ui_panel : engine_ui::get().get_panels())
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
