#include "actor_details_panel.h"

#include "engine_ui.h"
#include "components/common/actor_name_component_panel.h"
#include "components/common/actor_transform_component_panel.h"

namespace luly::ui
{
    actor_details_panel::actor_details_panel() : ui_panel("actor_details_panel")
    {
        initialize_component_panels();
    }

    actor_details_panel::~actor_details_panel()
    {
    }

    void actor_details_panel::on_render_panel()
    {
        ImGui::Begin("Actor Details", &m_show);
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

    void actor_details_panel::initialize_component_panels()
    {
        /* Common Components*/
        m_component_panels.push_back(std::make_shared<actor_name_component_panel>());
        m_component_panels.push_back(std::make_shared<actor_transform_component_panel>());
    }
}
