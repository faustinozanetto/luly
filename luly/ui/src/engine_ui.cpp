#include "engine_ui.h"

#include <renderer/renderer.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <logging/log.h>
#include <utils/assert.h>

#include "log/engine_ui_sink.h"
#include "panels/camera/camera_panel.h"
#include "panels/console/console_panel.h"
#include "panels/menubar/menubar_panel.h"
#include "panels/profiling/profiling_panel.h"
#include "panels/scene/scene_hierarchy_panel.h"
#include "panels/scene/actor/actor_details_panel.h"
#include "panels/viewport/viewport_panel.h"

namespace luly::ui
{
    engine_ui_data engine_ui::s_engine_ui_data = {};

    void engine_ui::initialize()
    {
        LY_TRACE("Engine UI initialization started...");
        initialize_panels();
        initialize_log_sink();
        LY_TRACE("Engine UI initialized successfully.");
    }

    engine_ui_data& engine_ui::get_ui_data()
    {
        return s_engine_ui_data;
    }

    void engine_ui::set_color_scheme()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
        colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.36f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.28f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.31f, 0.98f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.27f, 0.26f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.39f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.26f, 0.98f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.19f, 0.22f, 0.38f, 0.68f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.37f, 0.83f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.31f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.36f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.42f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.33f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.28f, 0.58f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.28f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.27f, 0.68f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.21f, 0.42f, 1.00f);
        colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.31f, 0.98f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.64f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 0.10f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.13f, 0.00f, 1.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.10f, 0.22f, 0.99f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
        colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(6.00f, 6.00f);
        style.FramePadding = ImVec2(5.00f, 2.00f);
        style.CellPadding = ImVec2(6.00f, 6.00f);
        style.ItemSpacing = ImVec2(6.00f, 6.00f);
        style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
        style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
        style.IndentSpacing = 25;
        style.ScrollbarSize = 15;
        style.GrabMinSize = 10;
        style.WindowBorderSize = 1;
        style.ChildBorderSize = 1;
        style.PopupBorderSize = 1;
        style.FrameBorderSize = 1;
        style.TabBorderSize = 1;
        style.WindowRounding = 7;
        style.ChildRounding = 4;
        style.FrameRounding = 3;
        style.PopupRounding = 4;
        style.ScrollbarRounding = 9;
        style.GrabRounding = 3;
        style.LogSliderDeadzone = 4;
        style.TabRounding = 4;
    }

    void engine_ui::initialize_panels()
    {
        s_engine_ui_data.panels.push_back(std::make_shared<menubar_panel>());
        s_engine_ui_data.panels.push_back(std::make_shared<console_panel>());
        s_engine_ui_data.panels.push_back(std::make_shared<viewport_panel>());
        s_engine_ui_data.panels.push_back(std::make_shared<profiling_panel>());
        s_engine_ui_data.panels.push_back(std::make_shared<scene_hierarchy_panel>());
        s_engine_ui_data.panels.push_back(std::make_shared<actor_details_panel>());
        s_engine_ui_data.panels.push_back(std::make_shared<camera_panel>());
    }

    void engine_ui::initialize_log_sink()
    {
        const spdlog::sink_ptr& imgui_sink = std::make_shared<engine_ui_sink<std::mutex>>();
        shared::log::add_sink(imgui_sink);
    }


    void engine_ui::set_render_target(uint32_t render_target)
    {
        s_engine_ui_data.render_target = render_target;
    }

    void engine_ui::set_selected_actor(const std::shared_ptr<scene::scene_actor>& selected_actor)
    {
        s_engine_ui_data.selected_actor = selected_actor;
    }

    void engine_ui::on_update()
    {
        ImGui::SetCurrentContext((ImGuiContext*)ImGui::GetCurrentContext());
        for (const auto& ui_panel : s_engine_ui_data.panels)
        {
            ui_panel->on_render_panel();
        }
    }
}
