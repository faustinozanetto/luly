#pragma once

#include "panels/ui_panel.h"
#include "renderer/renderer/pass/render_pass.h"

namespace luly::ui
{
    class renderer_panel : public ui_panel
    {
    public:
        renderer_panel();
        ~renderer_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        void render_tonemapping_details();
        void render_bloom_details();
        
        static bool s_show;

        std::vector<const char*> m_tonemapping_names;
    };
}
