#pragma once

#include "panels/scene/actor/components/actor_component_panel.h"
#include "renderer/animations/animation_clip.h"

namespace luly::ui
{
    class actor_animation_controller_component_panel : public actor_component_panel
    {
    public:
        actor_animation_controller_component_panel();

        /* Overrides */
        std::pair<bool, size_t> get_actor_component_details() override;
        void on_render_component_details() override;

    private:
        void draw_bone_children(const renderer::animation_clip_node& animation_clip_node);
    };
}
