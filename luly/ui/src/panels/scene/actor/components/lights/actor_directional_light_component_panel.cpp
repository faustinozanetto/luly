#include "actor_directional_light_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "utils/ui_utils.h"

#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/epsilon.hpp>

namespace luly::ui
{
    actor_directional_light_component_panel::actor_directional_light_component_panel() : actor_component_panel(
        "Directional Light Component")
    {
    }

    std::pair<bool, size_t> actor_directional_light_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<
            scene::directional_light_component>();
        auto component_hash = typeid(scene::directional_light_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_directional_light_component_panel::on_render_component_details()
    {
        const auto& directional_light_component = engine_ui::get_ui_data().selected_actor->get_component<
            scene::directional_light_component>();

        if (directional_light_component.get_directional_light())
        {
            auto& directional_light = directional_light_component.get_directional_light();
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            glm::vec3 color = directional_light->get_color();
            if (ui_utils::draw_property("Color", color, true))
            {
                directional_light->set_color(color);
            }

            glm::vec2 direction_angles = directional_light->get_direction_angles();
            if (ui_utils::draw_property("Direction", direction_angles, -180.0, 180.0))
            {
                if (glm::epsilonEqual(direction_angles.y, 0.0f, 1e-5f)) direction_angles.y = 1e-5f;

                directional_light->set_direction(direction_angles.x, direction_angles.y);
            }

            float intensity = directional_light->get_intensity();
            if (ui_utils::draw_property("Intensity", intensity, 0.0f, 5.0f, 0.01f))
            {
                directional_light->set_intensity(intensity);
            }
        }
        else
        {
            ImGui::Text("No light currently loaded!");
        }
    }
}
