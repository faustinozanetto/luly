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
            ImGui::AlignTextToFramePadding();
            if (ImGui::Button("Settings"))
                ImGui::OpenPopup("ConsoleSettings");

            if (ImGui::BeginPopup("ConsoleSettings"))
            {
                if (ImGui::Button("Clear"))
                {
                }
                ImGui::EndPopup();
            }

            for (const auto& message : m_messages)
            {
                ImGui::Text(message->get_message().c_str());
            }

            ImGui::End();
        }
    }

    void console_panel::add_message(const std::shared_ptr<console_panel_message>& message)
    {
        m_messages.push_back(message);
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
