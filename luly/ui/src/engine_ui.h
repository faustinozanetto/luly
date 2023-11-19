#pragma once

#include "panels/ui_panel.h"

#include <memory>
#include <vector>

namespace luly::ui
{
    struct engine_ui_data
    {
        uint32_t render_target;
        std::vector<std::shared_ptr<ui_panel>> panels;
    };

    class engine_ui
    {
    public:
        static void initialize();

        /* Getters */
        static engine_ui_data& get_ui_data();

        /* Methods*/
        static void begin_frame();
        static void end_frame();

        static void set_render_target(uint32_t render_target);

        static void on_update();

    private:
        static void set_color_scheme();
        static void initialize_imgui();
        static void initialize_panels();
        static void initialize_log_sink();

        static void begin_dockspace();
        static void end_dockspace();
    };
}
