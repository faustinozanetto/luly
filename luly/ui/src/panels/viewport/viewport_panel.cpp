#include "viewport_panel.h"

#include "engine_ui.h"

#include "math/math_utils.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/transform_component.h"
#include "utils/ui_utils.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui_internal.h>

namespace luly::ui
{
    bool viewport_panel::s_show = true;

    viewport_panel::viewport_panel() : ui_panel("viewport_panel")
    {
        m_tool_operations.push_back({"Universal", ICON_MDI_CROP_ROTATE, ImGuizmo::UNIVERSAL});
        m_tool_operations.push_back({"Translate",ICON_MDI_ARROW_ALL, ImGuizmo::TRANSLATE});
        m_tool_operations.push_back({"Rotate",ICON_MDI_ROTATE_ORBIT, ImGuizmo::ROTATE,});
        m_tool_operations.push_back({"Scale", ICON_MDI_ARROW_EXPAND_ALL, ImGuizmo::SCALE});
    }

    viewport_panel::~viewport_panel()
    {
    }

    void viewport_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Viewport", &s_show))
        {
            render_toolbar();
            render_scene_viewport();
            render_guizmos();

            ImGui::End();
        }
    }

    bool viewport_panel::get_show_panel()
    {
        return s_show;
    }

    void viewport_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }

    void viewport_panel::render_guizmos()
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
        if (!current_scene) return;

        const std::shared_ptr<renderer::perspective_camera>& perspective_camera = current_scene->
            get_camera_manager()->get_perspective_camera();

        glm::mat4 view_matrix = perspective_camera->get_view_matrix();
        glm::mat4 projection_matrix = perspective_camera->get_projection_matrix();

        render_transform_guizmo(view_matrix, projection_matrix);
        render_camera_guizmo(view_matrix, projection_matrix);

        perspective_camera->set_view_matrix(view_matrix);
        perspective_camera->set_projection_matrix(projection_matrix);
    }

    void viewport_panel::render_transform_guizmo(glm::mat4& view_matrix, glm::mat4& projection_matrix)
    {
        engine_ui& engine_ui = engine_ui::get();

        const std::shared_ptr<scene::scene_actor>& selected_actor = engine_ui.get_selected_actor();
        if (selected_actor && engine_ui.get_show_guizmos())
        {
            if (!selected_actor->has_component<scene::transform_component>()) return;

            const scene::transform_component& transform_component = selected_actor->get_component<
                scene::transform_component>();
            glm::mat4 transform = transform_component.get_transform()->get_transform();

            const float snap_values[3] = {
                engine_ui.get_snap_value(), engine_ui.get_snap_value(),
                engine_ui.get_snap_value()
            };

            ImGuizmo::Manipulate(glm::value_ptr(view_matrix), glm::value_ptr(projection_matrix),
                                 engine_ui.get_selected_operation(), ImGuizmo::LOCAL,
                                 glm::value_ptr(transform),
                                 nullptr, engine_ui.get_use_snap() ? snap_values : nullptr);

            // Update transform after ImGuizmo usage.
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

    void viewport_panel::render_camera_guizmo(glm::mat4& view_matrix, glm::mat4& projection_matrix)
    {
        const float window_width = ImGui::GetWindowWidth();
        const float window_height = ImGui::GetWindowHeight();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, window_width, window_height);
        const float view_manipulate_right = ImGui::GetWindowPos().x + window_width;
        const float view_manipulate_top = ImGui::GetWindowPos().y;

        ImGuizmo::ViewManipulate(glm::value_ptr(view_matrix), 8.0f,
                                 ImVec2(view_manipulate_right - 128, view_manipulate_top + 32), ImVec2(128, 128),
                                 0x10101010);
    }

    void viewport_panel::render_toolbar()
    {
        engine_ui& engine_ui = engine_ui::get();

        const ImGuiStyle style = ImGui::GetStyle();
        const float text_height = ImGui::CalcTextSize("A").y;
        const ImVec2 option_size = ImVec2{text_height, text_height} * 2.0f;

        const ImVec2 panel_position = ImGui::GetWindowPos() + ImGui::GetCursorPos() + style.WindowPadding;
        ImGui::SetNextWindowPos(panel_position);

        const ImGuiWindowFlags panel_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoScrollWithMouse |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_NoBringToFrontOnFocus;

        const ImGuiSelectableFlags option_flags = ImGuiSelectableFlags_NoPadWithHalfSpacing;

        if (ImGui::Begin("##TransformTools", nullptr, panel_flags))
        {
            ImGui::BeginGroup();
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.5f, 0.5f));

            for (operation_tool_data& operation_tool_data : m_tool_operations)
            {
                ImGui::PushID(operation_tool_data.operation);

                const bool is_selected = (operation_tool_data.operation == engine_ui.get_selected_operation());
                if (ImGui::Selectable(operation_tool_data.icon.c_str(), is_selected, option_flags, option_size))
                {
                    engine_ui.set_selected_operation(operation_tool_data.operation);
                }
                ui_utils::draw_tooltip(operation_tool_data.name.c_str());

                ImGui::PopID();

                ImGui::SameLine();
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
                ImGui::SameLine();
            }

            if (ImGui::Selectable(ICON_MDI_MAGNET, engine_ui.get_use_snap(), option_flags,
                                  option_size))
            {
                engine_ui.set_use_snap(!engine_ui.get_use_snap());
            }
            ui_utils::draw_tooltip("Use Snapping");

            ImGui::EndGroup();
            ImGui::PopStyleVar();

            ImGui::End();
        }
    }

    void viewport_panel::render_scene_viewport()
    {
        const ImVec2 scene_view_size = ImGui::GetContentRegionAvail();
        ImGui::Image(reinterpret_cast<ImTextureID>(engine_ui::get().get_render_target()), scene_view_size, ImVec2{0, 1},
                     ImVec2{1, 0});
    }
}
