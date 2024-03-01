#include "actor_spot_light_component_panel.h"

#include <glm/ext/scalar_constants.hpp>

#include "engine_ui.h"
#include "scene/actor/components/lights/spot_light_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_spot_light_component_panel::actor_spot_light_component_panel() : actor_component_panel(
        "Spot Light Component")
    {
    }

    std::pair<bool, size_t> actor_spot_light_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::spot_light_component>();
        auto component_hash = typeid(scene::spot_light_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_spot_light_component_panel::on_render_component_details()
    {
        const scene::spot_light_component& spot_light_component = engine_ui::get().get_selected_actor()->get_component<
            scene::spot_light_component>();

        if (spot_light_component.get_spot_light())
        {
            auto& spot_light = spot_light_component.get_spot_light();
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            glm::vec3 color = spot_light->get_color();
            if (ui_utils::draw_property("Color", color, true))
            {
                spot_light->set_color(color);
            }

            glm::vec3 direction = spot_light->get_direction();
            if (ui_utils::draw_property("Direction", direction, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
            {
                spot_light->set_direction(direction);
            }

            float inner_cone_angle = spot_light->get_inner_cone_angle();
            if (ui_utils::draw_property("Inner Cone Angle", inner_cone_angle, 0.0f,
                                        spot_light->get_outer_cone_angle() - 0.01f, 0.01f))
            {
                spot_light->set_inner_cone_angle(inner_cone_angle);
            }

            float outer_cone_angle = spot_light->get_outer_cone_angle();
            if (ui_utils::draw_property("Outer Cone Angle", outer_cone_angle, 0.0f,
                                        spot_light->get_outer_cone_angle() + 0.01f, 0.01f))
            {
                spot_light->set_outer_cone_angle(outer_cone_angle);
            }

            float intensity = spot_light->get_intensity();
            if (ui_utils::draw_property("Intensity", intensity, 0.0f, 180.0f, 0.01f))
            {
                spot_light->set_intensity(intensity);
            }
        }
        else
        {
            ImGui::Text("No light currently loaded!");
        }
    }
}
