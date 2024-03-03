#include "engine_ui.h"

#include "log/engine_ui_sink.h"

#include "panels/camera/camera_panel.h"
#include "panels/console/console_panel.h"
#include "panels/menubar/menubar_panel.h"
#include "panels/profiling/profiling_panel.h"
#include "panels/scene/scene_hierarchy_panel.h"
#include "panels/scene/actor/actor_details_panel.h"
#include "panels/viewport/viewport_panel.h"
#include "panels/assets/assets_panel.h"
#include "panels/renderer/renderer_panel.h"
#include "panels/shadows/shadows_panel.h"

#include "renderer/renderer/renderer.h"
#include "application/application.h"
#include "utils/IconsMaterialDesignIcons.h"

#include "events/event_dispatcher.h"
#include "input/input_manager.h"
#include "panels/editor/editor_panel.h"
#include "panels/physics/physics_panel.h"
#include "panels/renderer/render_passes_panel.h"
#include "utils/ui_utils.h"

#include <logging/log.h>
#include <utils/assert.h>

#include <ImGuizmo.h>
#include <imgui.h>
#include <imgui_internal.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <glm/gtc/type_ptr.hpp>

#include "panels/debug/debug_panel.h"
#include "panels/scene/scenes_panel.h"

namespace luly::ui
{
    engine_ui* engine_ui::s_instance = nullptr;

    engine_ui::engine_ui()
    {
        initialize();
    }

    engine_ui::~engine_ui()
    {
        shutdown();
    }

    void engine_ui::initialize()
    {
        LY_TRACE("Engine UI initialization started...");
        s_instance = this;
        initialize_data();
        initialize_imgui();
        initialize_panels();
        initialize_log_sink();
        LY_TRACE("Engine UI initialized successfully.");
    }

    void engine_ui::shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    engine_ui& engine_ui::get()
    {
        if (!s_instance) s_instance = new engine_ui();
        return *s_instance;
    }

    void engine_ui::set_color_scheme()
    {
        ImVec4* colors = ImGui::GetStyle().Colors;
        /*
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
        
                /*
                ImGuiStyle& style = ImGui::GetStyle();
        
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
                style.TabRounding = 4;*/
    }

    void engine_ui::initialize_data()
    {
        m_use_snap = true;
        m_show_guizmos = true;
        m_snap_value = 0.1f;
        m_guizmos_scale = 0.25f;
        m_selected_guizmo_operation = ImGuizmo::UNIVERSAL;
    }

    void engine_ui::initialize_panels()
    {
        m_panels.push_back(std::make_shared<menubar_panel>());
        m_panels.push_back(std::make_shared<console_panel>());
        m_panels.push_back(std::make_shared<viewport_panel>());
        m_panels.push_back(std::make_shared<profiling_panel>());
        m_panels.push_back(std::make_shared<scene_hierarchy_panel>());
        m_panels.push_back(std::make_shared<actor_details_panel>());
        m_panels.push_back(std::make_shared<camera_panel>());
        m_panels.push_back(std::make_shared<renderer_panel>());
        m_panels.push_back(std::make_shared<assets_panel>());
        m_panels.push_back(std::make_shared<shadows_panel>());
        m_panels.push_back(std::make_shared<editor_panel>());
        m_panels.push_back(std::make_shared<physics_panel>());
        m_panels.push_back(std::make_shared<render_passes_panel>());
        m_panels.push_back(std::make_shared<debug_panel>());
        m_panels.push_back(std::make_shared<scenes_panel>());
    }

    void engine_ui::initialize_log_sink()
    {
        const spdlog::sink_ptr& imgui_sink = std::make_shared<engine_ui_sink<std::mutex>>();
        shared::log::add_sink(imgui_sink);
    }

    void engine_ui::on_event(events::base_event& base_event)
    {
        events::event_dispatcher dispatcher(base_event);
        dispatcher.dispatch<
            events::mouse_button_pressed_event>(BIND_EVENT_FN(engine_ui::on_mouse_button_pressed_event));
        dispatcher.dispatch<
            events::mouse_button_released_event>(BIND_EVENT_FN(engine_ui::on_mouse_button_released_event));
    }

    void engine_ui::on_update()
    {
        // Check for key press.
        if (input::input_manager::is_key_pressed(input::key::d1))
        {
            m_selected_guizmo_operation = ImGuizmo::UNIVERSAL;
        }
        else if (input::input_manager::is_key_pressed(input::key::d2))
        {
            m_selected_guizmo_operation = ImGuizmo::TRANSLATE;
        }
        else if (input::input_manager::is_key_pressed(input::key::d3))
        {
            m_selected_guizmo_operation = ImGuizmo::ROTATE;
        }
        else if (input::input_manager::is_key_pressed(input::key::d4))
        {
            m_selected_guizmo_operation = ImGuizmo::SCALE;
        }
        else if (input::input_manager::is_key_pressed(input::key::d5))
        {
            m_use_snap = !m_use_snap;
        }

        // Render all panels.
        for (const std::shared_ptr<ui_panel>& ui_panel : m_panels)
        {
            ui_panel->on_render_panel();
        }
    }

    void engine_ui::begin_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        begin_dockspace();
    }

    void engine_ui::end_frame()
    {
        end_dockspace();
        // Rendering
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(renderer::renderer::get_viewport_size().x),
                                static_cast<float>(renderer::renderer::get_viewport_size().y));

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void engine_ui::initialize_imgui()
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        // Setup font.
        io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Inter-Medium.ttf", 14.0f);

        // Setup icons font.
        static const ImWchar icons_ranges[] = {ICON_MIN_MDI, ICON_MAX_MDI, 0};
        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        icons_config.GlyphOffset.y = 1.0f;
        icons_config.OversampleH = icons_config.OversampleV = 1;
        icons_config.GlyphMinAdvanceX = 4.0f;
        icons_config.SizePixels = 12.0f;
        io.Fonts->AddFontFromFileTTF("assets/fonts/materialdesignicons-webfont.ttf", 14.0f, &icons_config,
                                     icons_ranges);

        // Setup Platform/Renderer bindings
        GLFWwindow* window = core::application::get().get_window()->get_native_handle();
        if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        {
            LY_CRITICAL("ImGui init for GLFW failed.");
            LY_ASSERT(false)
        }
        if (!ImGui_ImplOpenGL3_Init("#version 450"))
        {
            LY_CRITICAL("ImGui init for OpenGL failed.");
            LY_ASSERT(false)
        }

        ImGuizmo::SetGizmoSizeClipSpace(m_guizmos_scale / 2.0f);
    }

    void engine_ui::begin_dockspace()
    {
        static bool open = true;
        static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_NoWindowMenuButton | ImGuiDockNodeFlags_NoCloseButton;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        const float toolbar_height = ImGui::GetFrameHeight();
        ImGui::SetNextWindowPos({viewport->Pos.x, viewport->Pos.y + toolbar_height});
        ImGui::SetNextWindowSize({viewport->Size.x, viewport->Size.y - toolbar_height});
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace", &open, window_flags);
        ImGui::PopStyleVar(3);

        const ImGuiID dockspace_id = ImGui::GetID("Dockspace");
        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
        }
    }

    void engine_ui::end_dockspace()
    {
        ImGui::End();
    }

    bool engine_ui::on_mouse_button_pressed_event(const events::mouse_button_pressed_event& mouse_button_pressed_event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[ui_utils::get_mouse_button_code_to_imgui(mouse_button_pressed_event.get_button_code())] = true;

        return false;
    }

    bool engine_ui::on_mouse_button_released_event(
        const events::mouse_button_released_event& mouse_button_released_event)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[ui_utils::get_mouse_button_code_to_imgui(mouse_button_released_event.get_button_code())] = false;

        return false;
    }
}
