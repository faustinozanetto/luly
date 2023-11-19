#include "console_panel.h"

namespace luly::ui
{
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
        ImGui::Begin("Console", &m_show);

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

    void console_panel::add_message(const std::shared_ptr<console_panel_message>& message)
    {
        m_messages.push_back(message);
    }
}
