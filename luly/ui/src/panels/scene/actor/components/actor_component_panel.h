#pragma once

#include "panels/ui_panel.h"

#include <utils/assert.h>

namespace luly::ui
{
    class LULY_UI_API actor_component_panel : public ui_panel
    {
    public:
        actor_component_panel(const std::string& component_name);
        ~actor_component_panel() override;

        /* Overrides */
        void on_render_panel() override;
        void set_show_panel(bool show_panel) override {}
        bool get_show_panel() override { return true; }

        virtual void on_render_component_details() = 0;
        virtual std::pair<bool, size_t> get_actor_component_details() = 0;

    protected:
        std::string m_component_name;
    };
}
