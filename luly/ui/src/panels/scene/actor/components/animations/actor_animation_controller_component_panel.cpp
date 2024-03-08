#include "actor_animation_controller_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/animations/animation_controller_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_animation_controller_component_panel::actor_animation_controller_component_panel() : actor_component_panel(
        "Animation Controller Component")
    {
    }

    std::pair<bool, size_t> actor_animation_controller_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<
            scene::animation_controller_component>();
        auto component_hash = typeid(scene::animation_controller_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_animation_controller_component_panel::on_render_component_details()
    {
        const scene::animation_controller_component& animation_controller_component = engine_ui::get().
            get_selected_actor()->
            get_component<
                scene::animation_controller_component>();

        const std::shared_ptr<renderer::animation_controller>& animation_controller = animation_controller_component.
            get_animation_controller();
        const std::shared_ptr<renderer::animation_clip>& animation_clip = animation_controller->get_animation_clip();

        ui_utils::draw_property("Animation Controls");

        if (ui_utils::draw_button("Play"))
        {
            animation_controller->play();
        }
        if (ui_utils::draw_button("Stop"))
        {
            animation_controller->stop();
        }
        if (ui_utils::draw_button("Pause"))
        {
            animation_controller->pause();
        }
        if (ui_utils::draw_button("Resume"))
        {
            animation_controller->resume();
        }

        ImGui::Separator();

        float speed_multiplier = animation_controller->get_speed_multiplier();
        if (ui_utils::draw_property("Speed Multiplier", speed_multiplier, 0.0f, 5.0f, 0.001f))
        {
            animation_controller->set_speed_multiplier(speed_multiplier);
        }

        float current_time = animation_controller->get_current_time();
        ui_utils::draw_property("Current Time", current_time, 0, 0, 0, true);

        ImGui::Separator();
        ui_utils::draw_property("Bones Hierarchy");
        draw_bone_children(animation_clip->get_root_node());
    }

    void actor_animation_controller_component_panel::draw_bone_children(
        const renderer::animation_clip_node& animation_clip_node)
    {
        if (ImGui::TreeNode(animation_clip_node.name.c_str()))
        {
            if (animation_clip_node.children_count > 0)
            {
                for (const renderer::animation_clip_node& children : animation_clip_node.children)
                {
                    draw_bone_children(children);
                }
            }
            ImGui::TreePop();
        }
    }
}
