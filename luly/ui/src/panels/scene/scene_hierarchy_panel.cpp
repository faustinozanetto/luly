#include "scene_hierarchy_panel.h"

#include "application/application.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/name_component.h"

namespace luly::ui
{
    scene_hierarchy_panel::scene_hierarchy_panel() : ui_panel("scene_hierarchy_panel")
    {
    }

    scene_hierarchy_panel::~scene_hierarchy_panel()
    {
    }

    void scene_hierarchy_panel::on_render_panel()
    {
        const auto& current_scene = core::application::get().get_scene_manager()->get_current_scene();

        ImGui::Begin("Hierarchy", &m_show);
        if (current_scene)
        {
            if (ImGui::BeginPopupContextWindow("create_actor", ImGuiPopupFlags_MouseButtonRight))
            {
                if (ImGui::MenuItem("Create Actor"))
                {
                    current_scene->create_actor("Empty Actor");
                }
                ImGui::EndPopup();
            }

            // No actors message.
            if (!current_scene->get_registry()->storage<entt::entity>().in_use())
            {
                ImGui::Text("There are no actors in the current scene!");
            }

            // Actor hierarchy render
            current_scene->get_registry()->each([&](entt::entity actor_handle)
            {
                const auto& name_component = current_scene->get_registry()->get<scene::name_component>(
                    actor_handle);

                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
                flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
                bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)actor_handle, flags,
                                              name_component.get_name().c_str());

                // Selection of actor
                if (ImGui::IsItemClicked())
                {
                    const scene::scene_actor selected_actor = {actor_handle, current_scene.get()};
                }

                if (open)
                {
                    ImGui::TreePop();
                }
            });
        }
        else
        {
            ImGui::Text("No active scene!");
        }
        ImGui::End();
    }
}
