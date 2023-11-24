﻿#include "actor_point_light_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_point_light_component_panel::actor_point_light_component_panel() : actor_component_panel(
        "point Light Component")
    {
    }

    std::pair<bool, size_t> actor_point_light_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<
            scene::point_light_component>();
        auto component_hash = typeid(scene::point_light_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_point_light_component_panel::on_render_component_details()
    {
        const auto& point_light_component = engine_ui::get_ui_data().selected_actor->get_component<
            scene::point_light_component>();

        if (point_light_component.get_point_light())
        {
            auto& point_light = point_light_component.get_point_light();
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            glm::vec3 color = point_light->get_color();
            if (ui_utils::draw_property("Color", color, true))
            {
                point_light->set_color(color);
            }
        }
        else
        {
            ImGui::Text("No light currently loaded!");
        }
    }
}