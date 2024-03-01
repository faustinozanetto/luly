#include "actor_physics_material_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_physics_material_component_panel::actor_physics_material_component_panel() : actor_component_panel(
        "Physics Material Component")
    {
    }

    std::pair<bool, size_t> actor_physics_material_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::physics_material_component>();
        auto component_hash = typeid(scene::physics_material_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_physics_material_component_panel::on_render_component_details()
    {
        const scene::physics_material_component& physics_material_component = engine_ui::get().get_selected_actor()->
            get_component<
                scene::physics_material_component>();

        float static_friction = physics_material_component.get_physics_material()->get_static_friction();
        if (ui_utils::draw_property("Static Friction", static_friction, 0.0f, 50.0f, 0.01f))
        {
            physics_material_component.get_physics_material()->set_static_friction(static_friction);
        }

        float dynamic_friction = physics_material_component.get_physics_material()->get_dynamic_friction();
        if (ui_utils::draw_property("Dynamic Friction", dynamic_friction, 0.0f, 50.0f, 0.01f))
        {
            physics_material_component.get_physics_material()->set_dynamic_friction(dynamic_friction);
        }

        float restitution = physics_material_component.get_physics_material()->get_restitution();
        if (ui_utils::draw_property("Restitution", restitution, 0.0f, 1.0f, 0.01f))
        {
            physics_material_component.get_physics_material()->set_restitution(restitution);
        }
    }
}
