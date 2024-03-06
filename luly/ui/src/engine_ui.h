#pragma once

#include "panels/ui_panel.h"

#include <scene/actor/scene_actor.h>
#include <ImGuizmo.h>
#include <memory>
#include <vector>

#include "events/mouse/mouse_button_pressed_event.h"
#include "events/mouse/mouse_button_released_event.h"

namespace luly::ui
{
    class engine_ui
    {
    public:
        engine_ui();
        ~engine_ui();

        /* Getters */
        static engine_ui& get();
        uint32_t get_render_target() const { return m_render_target; }
        const std::shared_ptr<scene::scene_actor>& get_selected_actor() const { return m_selected_actor; }
        bool get_show_guizmos() const { return m_show_guizmos; }
        bool get_use_snap() const { return m_use_snap; }
        float get_snap_value() const { return m_snap_value; }
        float get_guizmos_scale() const { return m_guizmos_scale; }
        ImGuizmo::OPERATION get_selected_operation() const { return m_selected_guizmo_operation; }
        const std::vector<std::shared_ptr<ui_panel>>& get_panels() const { return m_panels; }
        glm::vec2 get_min_bounds() const { return m_viewport_bounds[0]; }
        glm::vec2 get_max_bounds() const { return m_viewport_bounds[1]; }
        glm::vec2 get_viewport_size() const { return m_viewport_size; }

        /* Setters */
        void set_render_target(uint32_t render_target) { m_render_target = render_target; }

        void set_viewport_min_bounds(const glm::vec2& viewport_min_bounds)
        {
            m_viewport_bounds[0] = viewport_min_bounds;
        }

        void set_viewport_max_bounds(const glm::vec2& viewport_max_bounds)
        {
            m_viewport_bounds[1] = viewport_max_bounds;
        }

        void set_selected_actor(const std::shared_ptr<scene::scene_actor>& selected_actor)
        {
            m_selected_actor = selected_actor;
        }

        void set_viewport_size(const glm::vec2& viewport_size) { m_viewport_size = viewport_size; }

        void set_show_guizmos(bool show_guizmos) { m_show_guizmos = show_guizmos; }
        void set_use_snap(bool use_snap) { m_use_snap = use_snap; }
        void set_snap_value(float snap_value) { m_snap_value = snap_value; }
        void set_guizmos_scale(float guizmos_scale) { m_guizmos_scale = guizmos_scale; }
        void set_selected_operation(ImGuizmo::OPERATION operation) { m_selected_guizmo_operation = operation; }

        /* Methods */
        void on_event(events::base_event& base_event);
        void on_update();
        void begin_frame();
        void end_frame();

    private:
        void initialize();
        void shutdown();

        void set_color_scheme();
        void initialize_data();
        void initialize_panels();
        void initialize_log_sink();
        void initialize_imgui();

        void begin_dockspace();
        void end_dockspace();

        /* Events */
        bool on_mouse_button_pressed_event(const events::mouse_button_pressed_event& mouse_button_pressed_event);
        bool on_mouse_button_released_event(const events::mouse_button_released_event& mouse_button_released_event);

        uint32_t m_render_target;

        bool m_show_guizmos;
        bool m_use_snap;
        float m_snap_value;
        float m_guizmos_scale;
        ImGuizmo::OPERATION m_selected_guizmo_operation;

        glm::vec2 m_viewport_bounds[2];
        glm::vec2 m_viewport_size;

        std::vector<std::shared_ptr<ui_panel>> m_panels;
        std::shared_ptr<scene::scene_actor> m_selected_actor;

        static engine_ui* s_instance;
    };
}
