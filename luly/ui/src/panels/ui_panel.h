#pragma once

#include <imgui.h>
#include <string>

namespace luly::ui
{
    class ui_panel
    {
    public:
        ui_panel(std::string name) : m_name(std::move(name))
        {
        }

        virtual ~ui_panel() = default;

        /* Virtuals */
        virtual bool get_show_panel() = 0;
        virtual void set_show_panel(bool show_panel) =0;
        virtual void on_render_panel() = 0;

        /* Getters */
        const std::string& get_name() const { return m_name; }

    protected:
        std::string m_name;
    };
}
