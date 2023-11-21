#pragma once

#include "console_panel_message.h"
#include "panels/ui_panel.h"

#include <memory>
#include <vector>

namespace luly::ui
{
    class console_panel : public ui_panel
    {
    public:
        console_panel();
        ~console_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

        /* Getters*/
        static console_panel* get_instance() { return s_instance; }

        /* Methods */
        void add_message(const std::shared_ptr<console_panel_message>& message);

    private:
        std::vector<std::shared_ptr<console_panel_message>> m_messages;
        static console_panel* s_instance;
        static bool s_show;
    };
}
