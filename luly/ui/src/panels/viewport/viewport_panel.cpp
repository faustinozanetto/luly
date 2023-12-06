#include "viewport_panel.h"

#include "engine_ui.h"

#include "math/math_utils.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/transform_component.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui_internal.h>

namespace luly::ui
{
    bool viewport_panel::s_show = true;

    viewport_panel::viewport_panel() : ui_panel("viewport_panel")
    {
    }

    viewport_panel::~viewport_panel()
    {
    }

    void viewport_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Viewport", &s_show))
        {
            const ImGuiStyle style = ImGui::GetStyle();

            // Toolbar
            const float text_height = ImGui::CalcTextSize("A").y;
            int toolbarItems = 3;
            const ImVec2 toolbar_option_size = ImVec2{text_height, text_height} * 2.0f;
            const ImVec2 toolbar_position = ImGui::GetWindowPos() + ImGui::GetCursorPos();
            const ImVec2 toolbar_size = {
                toolbar_option_size.x + style.WindowPadding.x * 2.0f, //
                toolbar_option_size.y * toolbarItems + style.WindowPadding.y * 2.0f
            };
            ImGui::SetNextWindowPos(toolbar_position);
            ImGui::SetNextWindowSize(toolbar_size);

            const ImGuiWindowFlags toolbar_flags = ImGuiWindowFlags_NoDecoration | //
                ImGuiWindowFlags_NoMove | //
                ImGuiWindowFlags_NoScrollWithMouse | //
                ImGuiWindowFlags_NoSavedSettings | //
                ImGuiWindowFlags_NoBringToFrontOnFocus;

            const ImGuiSelectableFlags toolbar_option_flags = ImGuiSelectableFlags_NoPadWithHalfSpacing;

            if (ImGui::Begin("##ViewportToolbar", nullptr, toolbar_flags))
            {
                // Bring the toolbar window always on top.
                ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindow());

                ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));
                if (ImGui::Selectable(ICON_FA_ARROWS_UP_DOWN_LEFT_RIGHT, m_selected_operation == ImGuizmo::TRANSLATE,
                                      toolbar_option_flags,
                                      toolbar_option_size))
                {
                    m_selected_operation = ImGuizmo::TRANSLATE;
                }
                if (ImGui::Selectable(ICON_FA_ARROWS_ROTATE, m_selected_operation == ImGuizmo::ROTATE,
                                      toolbar_option_flags,
                                      toolbar_option_size))
                {
                    m_selected_operation = ImGuizmo::ROTATE;
                }
                if (ImGui::Selectable(
                    ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER, m_selected_operation == ImGuizmo::SCALE,
                    toolbar_option_flags, toolbar_option_size))
                {
                    m_selected_operation = ImGuizmo::SCALE;
                }
                ImGui::PopStyleVar();
            }
            ImGui::End();

            render_scene_viewport();
            render_transform_guizmo();
        }
        ImGui::End();
    }

    bool viewport_panel::get_show_panel()
    {
        return s_show;
    }

    void viewport_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }

    void viewport_panel::render_transform_guizmo() const
    {
        const ImVec2 min_region = ImGui::GetWindowContentRegionMin();
        const ImVec2 max_region = ImGui::GetWindowContentRegionMax();
        const ImVec2 offset = ImGui::GetWindowPos();
        glm::vec2 bounds[2];
        bounds[0] = {min_region.x + offset.x, min_region.y + offset.y};
        bounds[1] = {max_region.x + offset.x, max_region.y + offset.y};

        ImGuizmo::SetRect(bounds[0].x, bounds[0].y, bounds[1].x - bounds[0].x, bounds[1].y - bounds[0].y);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetOrthographic(false);

        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        const std::shared_ptr<renderer::perspective_camera>& perspective_camera = current_scene->
            get_camera_manager()->get_perspective_camera();

        glm::mat4 view_matrix = perspective_camera->get_view_matrix();
        glm::mat4 projection_matrix = perspective_camera->get_projection_matrix();

        const std::shared_ptr<scene::scene_actor>& selected_actor = engine_ui::get_ui_data().selected_actor;
        if (selected_actor)
        {
            const scene::transform_component& transform_component = selected_actor->get_component<
                scene::transform_component>();
            glm::mat4 transform = transform_component.get_transform()->get_transform();

            ImGuizmo::Manipulate(glm::value_ptr(view_matrix), glm::value_ptr(projection_matrix),
                                 m_selected_operation, ImGuizmo::WORLD, glm::value_ptr(transform),
                                 nullptr, nullptr);

            // Update transform after imguizmo usage.
            if (ImGuizmo::IsUsing())
            {
                glm::vec3 translation, scale;
                glm::quat rotation;
                math::math_utils::decompose_transform(transform, translation, rotation, scale);

                // Update the transform of the selected actor.
                transform_component.get_transform()->set_location(translation);
                transform_component.get_transform()->set_rotation(rotation);
                transform_component.get_transform()->set_scale(scale);
            }
        }
    }

    void viewport_panel::render_scene_viewport()
    {
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        const uint32_t render_target = engine_ui::get_ui_data().render_target;
        ImGui::Image(reinterpret_cast<ImTextureID>(render_target),
                     ImVec2{viewportPanelSize.x, viewportPanelSize.y}, ImVec2{0, 1},
                     ImVec2{1, 0});
    }
}
