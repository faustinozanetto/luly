#include "actor_name_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/name_component.h"

namespace luly::ui
{
    actor_name_component_panel::actor_name_component_panel() : actor_component_panel("Name Component")
    {
    }

    std::pair<bool, size_t> actor_name_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<scene::name_component>();
        auto component_hash = typeid(scene::name_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_name_component_panel::on_render_component_details()
    {
        auto& name_component = engine_ui::get_ui_data().selected_actor->get_component<scene::name_component>();

        // Editable name field.
        char buffer[256] = {};
        strcpy_s(buffer, name_component.get_name().c_str());
        if (ImGui::InputText("##name", buffer, sizeof(buffer)))
        {
            name_component.set_name(std::string(buffer));
        }

        // Actor handle
        ImGui::SameLine();
        ImGui::Text("Actor Handle: %s",
                    std::to_string(static_cast<uint32_t>(engine_ui::get_ui_data().selected_actor->get_handle())).
                    c_str());
    }
}
