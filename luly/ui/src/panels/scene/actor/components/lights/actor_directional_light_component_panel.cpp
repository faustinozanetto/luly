#include "actor_directional_light_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "utils/ui_utils.h"

#include <glm/ext/scalar_constants.hpp>

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

            glm::vec3 direction = directional_light->get_direction();
            if (ui_utils::draw_property("Direction", direction, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f))
            {
                directional_light->set_direction(direction);
            }

            float intensity = directional_light->get_intensity();
            if (ui_utils::draw_property("Intensity", intensity, 0.0f, 5.0f, 0.01f))
            {
                directional_light->set_intensity(intensity);
            }

            float distance = directional_light->get_distance();
            if (ui_utils::draw_property("Distance", distance, 0.0f, 150.0f, 0.01f))
            {
                directional_light->set_distance(distance);
            }

            ui_utils::draw_property("Cascaded Shadows");
            ImGui::Separator();
            float shadow_bias = directional_light->get_shadow_bias();
            if (ui_utils::draw_property("Shadow Bias", shadow_bias, 0.0f, 5.0f, 0.00001f))
            {
                directional_light->set_shadow_bias(shadow_bias);
            }

            float inverse_cascade_factor = directional_light->get_inverse_cascade_factor();
            if (ui_utils::draw_property("Inverse Cascade Factor", inverse_cascade_factor, 0.0f, 5.0f, 0.00001f))
            {
                directional_light->set_inverse_cascade_factor(inverse_cascade_factor);
            }

            float z_multiplier = directional_light->get_z_multiplier();
            if (ui_utils::draw_property("Z Multiplier", z_multiplier, 0.0f, 50.0f, 0.01f))
            {
                directional_light->set_z_multiplier(z_multiplier);
            }

            bool soft_shadows = directional_light->get_soft_shadows();
            if (ui_utils::draw_property("Soft Shadows", soft_shadows))
            {
                directional_light->set_soft_shados(soft_shadows);
            }
        }
        else
        {
            ImGui::Text("No light currently loaded!");
        }
    }
}
