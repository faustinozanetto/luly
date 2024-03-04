#include "actor_details_panel.h"

#include "engine_ui.h"

#include "components/common/actor_name_component_panel.h"
#include "components/common/actor_transform_component_panel.h"
#include "components/lights/actor_directional_light_component_panel.h"
#include "components/lights/actor_point_light_component_panel.h"
#include "components/lights/actor_spot_light_component_panel.h"
#include "components/physics/actors/actor_physics_dynamic_actor_component_panel.h"
#include "components/physics/actor_physics_material_component_panel.h"
#include "components/physics/actors/actor_physics_static_actor_component_panel.h"
#include "components/physics/collision_shapes/actor_physics_box_collision_component_panel.h"
#include "components/physics/collision_shapes/actor_physics_plane_collision_component_panel.h"
#include "components/physics/collision_shapes/actor_physics_sphere_collision_component_panel.h"
#include "components/rendering/actor_material_component_panel.h"
#include "components/rendering/actor_model_renderer_component_panel.h"
#include "components/rendering/actor_skybox_component_panel.h"

namespace luly::ui
{
    bool actor_details_panel::s_show = true;

    actor_details_panel::actor_details_panel() : ui_panel("actor_details_panel")
    {
        initialize_component_panels();
    }

    actor_details_panel::~actor_details_panel()
    {
    }

    void actor_details_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Actor Details", &s_show))
        {
            if (engine_ui::get().get_selected_actor())
            {
                for (const std::shared_ptr<actor_component_panel>& component_panel : m_component_panels)
                {
                    component_panel->on_render_panel();
                }
            }
            else
            {
                ImGui::Text("No selected actor!");
            }
        }
        ImGui::End();
    }

    void actor_details_panel::initialize_component_panels()
    {
        // Adding component panels
        ADD_COMPONENT_PANEL(actor_name_component_panel)
        ADD_COMPONENT_PANEL(actor_transform_component_panel)
        /* Rendering Components */
        ADD_COMPONENT_PANEL(actor_model_renderer_component_panel)
        ADD_COMPONENT_PANEL(actor_material_component_panel)
        ADD_COMPONENT_PANEL(actor_skybox_component_panel)
        /* Lights Components */
        ADD_COMPONENT_PANEL(actor_directional_light_component_panel)
        ADD_COMPONENT_PANEL(actor_point_light_component_panel)
        ADD_COMPONENT_PANEL(actor_spot_light_component_panel)
        /* Physics Components */
        ADD_COMPONENT_PANEL(actor_physics_material_component_panel)
        ADD_COMPONENT_PANEL(actor_physics_static_actor_component_panel)
        ADD_COMPONENT_PANEL(actor_physics_dynamic_actor_component_panel)
        ADD_COMPONENT_PANEL(actor_physics_box_collision_component_panel)
        ADD_COMPONENT_PANEL(actor_physics_sphere_collision_component_panel)
        ADD_COMPONENT_PANEL(actor_physics_plane_collision_component_panel)
    }

    bool actor_details_panel::get_show_panel()
    {
        return s_show;
    }

    void actor_details_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
