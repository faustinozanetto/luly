#include "scenes_panel.h"

#include "engine_ui.h"
#include <scene/scene_manager.h>
#include <scene/actor/components/name_component.h>

#include "utils/ui_utils.h"

namespace luly::ui
{
    bool scenes_panel::s_show = true;

    scenes_panel::scenes_panel() : ui_panel("scenes_panel")
    {
    }

    scenes_panel::~scenes_panel()
    {
    }

    void scenes_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Scenes", &s_show))
        {
            scene::scene_manager& scene_manager = scene::scene_manager::get();

            for (const std::shared_ptr<scene::scene>& scene : scene_manager.get_scenes())
            {
                const bool is_current_scene = scene_manager.get_current_scene() && scene->get_name() == scene_manager.
                    get_current_scene()->get_name();
                ImGuiTreeNodeFlags flags = (is_current_scene
                                                ? ImGuiTreeNodeFlags_Selected
                                                : 0) | ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_FramePadding;
                flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
                const bool open = ImGui::TreeNodeEx(scene->get_name().c_str(), flags);

                if (ImGui::IsItemClicked())
                {
                }

                if (open)
                {
                    ui_utils::draw_property("Is Current", is_current_scene ? "Yes" : "No");
                    if (ImGui::Button("Switch To"))
                    {
                        scene_manager.switch_scene(scene);
                    }
                    ImGui::TreePop();
                }
            }

            ImGui::End();
        }
    }

    bool scenes_panel::get_show_panel()
    {
        return s_show;
    }

    void scenes_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
