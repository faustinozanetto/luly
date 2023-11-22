#pragma once

#include "panels/ui_panel.h"

#include "ui_api.h"

#include <scene/actor/scene_actor.h>
#include <memory>
#include <vector>

namespace luly::ui
{
    struct LULY_UI_API engine_ui_data
    {
        uint32_t render_target;
        std::vector<std::shared_ptr<ui_panel>> panels;
        std::shared_ptr<scene::scene_actor> selected_actor;
    };

    class LULY_UI_API engine_ui
    {
    public:
        static void initialize();

        /* Getters */
        static engine_ui_data& get_ui_data();

        /* Setters */
        static void set_render_target(uint32_t render_target);
        static void set_selected_actor(const std::shared_ptr<scene::scene_actor>& selected_actor);

        static void on_update();

    private:
        static void set_color_scheme();
        static void initialize_panels();
        static void initialize_log_sink();

        static engine_ui_data s_engine_ui_data;
    };
}
