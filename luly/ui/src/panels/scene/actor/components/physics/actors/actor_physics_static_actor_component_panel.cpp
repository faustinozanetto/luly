#include "actor_physics_static_actor_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/physics/actors/physics_static_actor_component.h"

namespace luly::ui
{
    actor_physics_static_actor_component_panel::actor_physics_static_actor_component_panel() : actor_component_panel(
        "Physics Static Actor Component")
    {
    }

    std::pair<bool, size_t> actor_physics_static_actor_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::physics_static_actor_component>();
        auto component_hash = typeid(scene::physics_static_actor_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_physics_static_actor_component_panel::on_render_component_details()
    {
        const scene::physics_static_actor_component& physics_static_actor_component = engine_ui::get().
            get_selected_actor()->
            get_component<
                scene::physics_static_actor_component>();
    }
}
