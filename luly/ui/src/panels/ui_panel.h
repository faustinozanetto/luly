#pragma once

#include <imgui.h>
#include <string>

namespace luly::ui
{
    class ui_panel
    {
    public:
        ui_panel(std::string name) : m_show(true), m_name(std::move(name))
        {
        }

        virtual ~ui_panel() = default;

        /* Getters */
        bool& get_show_panel() { return m_show; }
        const std::string& get_name() const { return m_name; }

        /* Setters */
        void set_show_panel(bool show_panel) { m_show = show_panel; }

        /* Functions */
        virtual void on_render_panel() = 0;

    protected:
        bool m_show;
        std::string m_name;
    };
}
