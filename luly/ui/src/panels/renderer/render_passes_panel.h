#pragma once
#include <memory>

#include "panels/ui_panel.h"
#include "renderer/renderer/pass/render_pass.h"
#include <memory>

namespace luly::ui
{
    class render_passes_panel : public ui_panel
    {
    public:
        render_passes_panel();
        ~render_passes_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        bool draw_render_pass_details(const std::shared_ptr<renderer::render_pass>& render_pass);
        void draw_render_pass_outputs(
            const std::unordered_map<std::string, renderer::render_pass_output>& pass_outputs);

        static bool s_show;
    };
}
