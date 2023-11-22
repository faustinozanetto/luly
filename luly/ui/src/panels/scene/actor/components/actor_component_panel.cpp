#include "actor_component_panel.h"

#include <imgui_internal.h>


#include "application/application.h"

namespace luly::ui
{
    actor_component_panel::actor_component_panel(const std::string& component_name): ui_panel("editor_component_panel")
    {
        m_component_name = component_name;
    }

    actor_component_panel::~actor_component_panel()
    {
    }

    void actor_component_panel::on_render_panel()
    {
        auto current_scene = core::application::get().get_scene_manager();

        auto [has_component, component_hash] = get_actor_component_details();
        if (!has_component)
            return;

        // Render content of the component
        const ImGuiTreeNodeFlags component_flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        const ImVec2 available_region = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        const float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx(reinterpret_cast<void*>(component_hash), component_flags,
                                      m_component_name.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(available_region.x - line_height * 0.5f);

        // Settings Popup Open
        if (ImGui::Button("+", ImVec2{line_height, line_height}))
        {
            ImGui::OpenPopup("Settings");
        }

        // Render settings popup
        bool should_remove_component = false;
        if (ImGui::BeginPopup("Settings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                should_remove_component = true;
            }
            ImGui::EndPopup();
        }

        // Render actual component details
        if (open)
        {
            on_render_component_details();
            ImGui::TreePop();
        }
    }
}
