#include "actor_physics_dynamic_actor_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/physics/actors/physics_dynamic_actor_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_physics_dynamic_actor_component_panel::actor_physics_dynamic_actor_component_panel() : actor_component_panel(
        "Physics Dynamic Actor Component")
    {
    }

    std::pair<bool, size_t> actor_physics_dynamic_actor_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::physics_dynamic_actor_component>();
        auto component_hash = typeid(scene::physics_dynamic_actor_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_physics_dynamic_actor_component_panel::on_render_component_details()
    {
        const scene::physics_dynamic_actor_component& physics_dynamic_actor_component = engine_ui::get().
            get_selected_actor()->
            get_component<
                scene::physics_dynamic_actor_component>();

        float mass = physics_dynamic_actor_component.get_dynamic_actor()->get_mass();
        if (ui_utils::draw_property("Mass", mass, 0.01f, 100.0f, 0.01f))
        {
            physics_dynamic_actor_component.get_dynamic_actor()->set_mass(mass);
        }

        float linear_damping = physics_dynamic_actor_component.get_dynamic_actor()->get_linear_damping();
        if (ui_utils::draw_property("Linear Damping", linear_damping, 0.0f, 100.0f, 0.01f))
        {
            physics_dynamic_actor_component.get_dynamic_actor()->set_linear_damping(linear_damping);
        }

        float angular_damping = physics_dynamic_actor_component.get_dynamic_actor()->get_angular_damping();
        if (ui_utils::draw_property("Angular Damping", angular_damping, 0.0f, 100.0f, 0.01f))
        {
            physics_dynamic_actor_component.get_dynamic_actor()->set_angular_damping(angular_damping);
        }

        bool is_kinematic = physics_dynamic_actor_component.get_dynamic_actor()->get_is_kinematic();
        if (ui_utils::draw_property("Is Kinematic", is_kinematic))
        {
            physics_dynamic_actor_component.get_dynamic_actor()->set_kinematic(is_kinematic);
        }
    }
}
