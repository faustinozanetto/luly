﻿#include "actor_model_renderer_component_panel.h"

#include "engine_ui.h"
#include "scene/actor/components/model_renderer_component.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_model_renderer_component_panel::actor_model_renderer_component_panel() : actor_component_panel(
        "Model Renderer Component")
    {
    }

    std::pair<bool, size_t> actor_model_renderer_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<scene::model_renderer_component>();
        auto component_hash = typeid(scene::model_renderer_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_model_renderer_component_panel::on_render_component_details()
    {
        const auto& model_renderer_component = engine_ui::get_ui_data().selected_actor->get_component<
            scene::model_renderer_component>();

        if (model_renderer_component.get_model())
        {
            ui_utils::draw_property("Mesh Count",
                                    std::format("{}", model_renderer_component.get_model()->get_meshes().size()));
            if (ImGui::TreeNode("Meshes"))
            {
                for (const auto& mesh : model_renderer_component.get_model()->get_meshes())
                {
                    if (ImGui::TreeNode(std::format("Mesh '{}'", mesh->get_name()).c_str()))
                    {
                        ImGui::Text("Vertices %llu", mesh->get_vertices().size());
                        ImGui::Text("Indices %llu", mesh->get_indices().size());

                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }
        else
        {
            ImGui::Text("No model currently loaded!");
        }
    }
}
