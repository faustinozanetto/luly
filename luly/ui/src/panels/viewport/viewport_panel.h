#pragma once

#include "panels/ui_panel.h"

#include <ImGuizmo.h>
#include <vector>

namespace luly::ui
{
    struct operation_tool_data
    {
        std::string name;
        std::string icon;
        ImGuizmo::OPERATION operation;
    };

    class viewport_panel : public ui_panel
    {
    public:
        viewport_panel();
        ~viewport_panel() override;

        /* Overrides */
        void on_render_panel() override;
        bool get_show_panel() override;
        void set_show_panel(bool show_panel) override;

    private:
        void render_transform_guizmo() const;
        void render_toolbar();
        void render_scene_viewport();

        static bool s_show;

        std::vector<operation_tool_data> m_tool_operations;
    };
}
