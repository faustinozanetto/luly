#include "actor_details_panel.h"

#include "engine_ui.h"

#include "components/common/actor_name_component_panel.h"
#include "components/common/actor_transform_component_panel.h"
#include "components/lights/actor_directional_light_component_panel.h"
#include "components/rendering/actor_material_component_panel.h"
#include "components/rendering/actor_model_renderer_component_panel.h"

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
            if (engine_ui::get_ui_data().selected_actor)
            {
                for (const auto& component_panel : m_component_panels)
                {
                    component_panel->on_render_panel();
                }
            }
            else
            {
                ImGui::Text("No selected actor!");
            }
            ImGui::End();
        }
    }

    void actor_details_panel::initialize_component_panels()
    {
        /* Common Components */
        m_component_panels.push_back(std::make_shared<actor_name_component_panel>());
        m_component_panels.push_back(std::make_shared<actor_transform_component_panel>());
        /* Rendering Components */
        m_component_panels.push_back(std::make_shared<actor_model_renderer_component_panel>());
        m_component_panels.push_back(std::make_shared<actor_material_component_panel>());
        /* Lights Components */
        m_component_panels.push_back(std::make_shared<actor_directional_light_component_panel>());
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
