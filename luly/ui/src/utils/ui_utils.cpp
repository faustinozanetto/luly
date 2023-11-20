#include "ui_utils.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace luly::ui
{
    void ui_utils::draw_property(const std::string& name, const std::string& content)
    {
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        ImGui::Text(content.c_str());

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
    }

    bool ui_utils::draw_property(const std::string& name, glm::vec2& value, float min, float max,
                                 float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});

        float lineHeight = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
        {
            value.x = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f"))
            modified = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
        {
            value.y = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f"))
            modified = true;
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PopStyleVar();

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool ui_utils::draw_property(const std::string& name, glm::vec3& value, float min, float max,
                                 float step, float reset_value)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 5});

        float lineHeight = ImGui::GetTextLineHeightWithSpacing();
        ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        if (ImGui::Button("X", buttonSize))
        {
            value.x = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##X", &value.x, step, min, max, "%.2f"))
        {
            modified = true;
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        if (ImGui::Button("Y", buttonSize))
        {
            value.y = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##Y", &value.y, step, min, max, "%.2f"))
        {
            modified = true;
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        if (ImGui::Button("Z", buttonSize))
        {
            value.z = reset_value;
            modified = true;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        if (ImGui::DragFloat("##Z", &value.z, step, min, max, "%.2f"))
        {
            modified = true;
        }
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool ui_utils::draw_property(const std::string& name, glm::vec3& value, bool color)
    {
        bool modified = false;
        ImGui::PushID(name.c_str());
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::ColorEdit3(id.c_str(), glm::value_ptr(value)))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }

    bool ui_utils::draw_property(const std::string& name, glm::vec4& value, bool color)
    {
        bool modified = false;
        // Name
        ImGui::AlignTextToFramePadding();
        ImGui::Columns(2);
        ImGui::PushItemWidth(-1);
        ImGui::TextUnformatted(name.c_str());
        ImGui::NextColumn();

        // Content
        const std::string id = "##" + name;
        if (ImGui::ColorEdit4(id.c_str(), glm::value_ptr(value)))
        {
            modified = true;
        }

        // Reset
        ImGui::Columns(1);
        ImGui::PopID();
        return modified;
    }
}
