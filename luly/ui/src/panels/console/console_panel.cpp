#include "console_panel.h"

namespace luly::ui
{
    bool console_panel::s_show = true;
    console_panel* console_panel::s_instance = nullptr;

    console_panel::console_panel() : ui_panel("console_panel")
    {
        s_instance = this;
    }

    console_panel::~console_panel()
    {
    }

    void console_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Console", &s_show))
        {
            if (ImGui::BeginTable("Messages", 3,
                                  ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders |
                                  ImGuiTableFlags_SizingStretchProp | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg))
            {
                ImGui::TableSetupColumn("Level", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 0);
                ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed, 0.0f, 1);
                ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_NoSort, 0.0f, 2);
                ImGui::TableSetupScrollFreeze(0, 1);
                
                ImGui::TableHeadersRow();

                ImGui::TableNextRow();

                for (const std::shared_ptr<console_panel_message>& message : m_messages)
                {
                    // Level
                    ImGui::TableNextColumn();
                    ImGui::PushStyleColor(ImGuiCol_Text, get_level_color(message->get_level()));
                    ImGui::TextUnformatted(get_level_to_string(message->get_level()));
                    ImGui::PopStyleColor();

                    // Time
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(message->get_time().c_str());

                    // Message
                    ImGui::TableNextColumn();
                    ImGui::TextUnformatted(message->get_message().c_str());
                    ImGui::TableNextRow();
                }
                ImGui::EndTable();
            }
            ImGui::End();
        }
    }

    void console_panel::add_message(const std::shared_ptr<console_panel_message>& message)
    {
        m_messages.push_back(message);
    }

    ImVec4 console_panel::get_level_color(spdlog::level::level_enum level)
    {
        switch (level)
        {
        case spdlog::level::level_enum::trace:
            return {0.8f, 0.8f, 0.8f, 1.0f};
        case spdlog::level::level_enum::debug:
            return {0.0f, 1.0f, 1.0f, 1.0f};
        case spdlog::level::level_enum::info:
            return {1.0f, 1.0f, 1.0f, 1.0f};
        case spdlog::level::level_enum::warn:
            return {1.0f, 0.8f, 0.0f, 1.0f};
        case spdlog::level::level_enum::err:
            return {1.0f, 0.0f, 0.0f, 1.0f};
        case spdlog::level::level_enum::critical:
            return {0.8f, 0.0f, 0.0f, 1.0f};
        case spdlog::level::level_enum::off:
            return {0.0f, 0.0f, 0.0f, 1.0f};
        default:
            return {1.0f, 1.0f, 1.0f, 1.0f};
        }
    }

    const char* console_panel::get_level_to_string(spdlog::level::level_enum level)
    {
        switch (level)
        {
        case spdlog::level::level_enum::trace:
            return "trace";
        case spdlog::level::level_enum::debug:
            return "debug";
        case spdlog::level::level_enum::info:
            return "info";
        case spdlog::level::level_enum::warn:
            return "warn";
        case spdlog::level::level_enum::err:
            return "error";
        case spdlog::level::level_enum::critical:
            return "critical";
        case spdlog::level::level_enum::off:
            return "off";
        default:
            return "unknown";
        }
    }

    bool console_panel::get_show_panel()
    {
        return s_show;
    }

    void console_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
