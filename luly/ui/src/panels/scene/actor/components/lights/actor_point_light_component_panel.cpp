#include "actor_point_light_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_point_light_component_panel::actor_point_light_component_panel() : actor_component_panel(
        "Point Light Component")
    {
    }

    std::pair<bool, size_t> actor_point_light_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::point_light_component>();
        auto component_hash = typeid(scene::point_light_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_point_light_component_panel::on_render_component_details()
    {
        scene::point_light_component& point_light_component = engine_ui::get().get_selected_actor()->get_component<
            scene::point_light_component>();

        if (point_light_component.get_point_light())
        {
            const std::shared_ptr<renderer::point_light>& point_light = point_light_component.get_point_light();
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            glm::vec3 color = point_light->get_color();
            if (ui_utils::draw_property("Color", color, true))
            {
                point_light->set_color(color);
            }

            float intensity = point_light->get_intensity();
            if (ui_utils::draw_property("Intensity", intensity, 0.0f, 5.0f, 0.01f))
            {
                point_light->set_intensity(intensity);
            }

            ui_utils::draw_property("Attenuation");
            ImGui::Separator();
            float constant_factor = point_light->get_constant_factor();
            if (ui_utils::draw_property("Constant Factor", constant_factor, 0.0f, 2.0f, 0.001f))
            {
                point_light->set_constant_factor(constant_factor);
            }

            float linear_factor = point_light->get_linear_factor();
            if (ui_utils::draw_property("Linear Factor", linear_factor, 0.0f, 2.0f, 0.001f))
            {
                point_light->set_linear_factor(linear_factor);
            }

            float quadratic_factor = point_light->get_quadratic_factor();
            if (ui_utils::draw_property("Quadratic Factor", quadratic_factor, 0.0f, 2.0f, 0.001f))
            {
                point_light->set_quadratic_factor(quadratic_factor);
            }

            ui_utils::draw_property("Shadow Mapping");
            ImGui::Separator();

            float shadow_map_far_plane = point_light->get_shadow_map_far_plane();
            if (ui_utils::draw_property("Shadow Map Far Plane", shadow_map_far_plane, 0.01f, 350.0f, 0.01f))
            {
                point_light->set_shadow_map_far_plane(shadow_map_far_plane);
            }

            bool enable_shadows = point_light_component.get_enable_shadows();
            if (ui_utils::draw_property("Enable Shadows", enable_shadows))
            {
                point_light_component.set_enable_shadows(enable_shadows);
            }
        }
        else
        {
            ImGui::Text("No light currently loaded!");
        }
    }
}
