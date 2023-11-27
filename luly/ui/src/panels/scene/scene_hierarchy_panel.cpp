#include "scene_hierarchy_panel.h"

#include "engine_ui.h"
#include <application/application.h>
#include <scene/scene_manager.h>
#include <scene/actor/components/name_component.h>

namespace luly::ui
{
    bool scene_hierarchy_panel::s_show = true;

    scene_hierarchy_panel::scene_hierarchy_panel() : ui_panel("scene_hierarchy_panel")
    {
    }

    scene_hierarchy_panel::~scene_hierarchy_panel()
    {
    }

    void scene_hierarchy_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Scene Hierarchy", &s_show))
        {
            const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
            const std::shared_ptr<scene::scene_actor>& current_actor = engine_ui::get_ui_data().selected_actor;

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

                    const bool is_current_actor_selected = current_actor != nullptr && current_actor->get_handle() ==
                        actor_handle;
                    ImGuiTreeNodeFlags flags = (is_current_actor_selected
                                                    ? ImGuiTreeNodeFlags_Selected
                                                    : 0) | ImGuiTreeNodeFlags_OpenOnArrow |
                        ImGuiTreeNodeFlags_FramePadding;
                    flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
                    const bool open = ImGui::TreeNodeEx(
                        reinterpret_cast<void*>(static_cast<uint64_t>(static_cast<uint32_t>(actor_handle))), flags,
                        name_component.get_name().c_str());

                    // Selection of actor
                    if (ImGui::IsItemClicked())
                    {
                        engine_ui::set_selected_actor(current_scene->get_actor(actor_handle));
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

    bool scene_hierarchy_panel::get_show_panel()
    {
        return s_show;
    }

    void scene_hierarchy_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
