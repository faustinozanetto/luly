#include "debug_panel.h"

#include "engine_ui.h"
#include "renderer/renderer/debug/debug_renderer.h"

#include "utils/ui_utils.h"

namespace luly::ui
{
    bool debug_panel::s_show = true;

    debug_panel::debug_panel() : ui_panel("debug_panel")
    {
    }

    debug_panel::~debug_panel()
    {
    }

    void debug_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Debug", &s_show))
        {
            renderer::debug_renderer_data& debug_renderer_data = renderer::debug_renderer::get_data();

            float lines_thickness = debug_renderer_data.lines_thickness;
            if (ui_utils::draw_property("Lines Thickness", lines_thickness, 0.0f, 10.0f, 0.1f))
            {
                renderer::debug_renderer::set_lines_thickness(lines_thickness);
            }

            glm::vec3 physics_box_collisions_color = debug_renderer_data.physics_box_collisions_color;
            if (ui_utils::draw_property("Physics Box Collisions Color", physics_box_collisions_color, true))
            {
                renderer::debug_renderer::set_physics_box_collisions_color(physics_box_collisions_color);
            }

            bool enable_physics_box_collisions = debug_renderer_data.enable_physics_box_collisions;
            if (ui_utils::draw_property("Enable Physics Box Collisions", enable_physics_box_collisions))
            {
                renderer::debug_renderer::set_physics_box_collisions_enabled(enable_physics_box_collisions);
            }

            glm::vec3 physics_sphere_collisions_color = renderer::debug_renderer::get_data().
                physics_sphere_collisions_color;
            if (ui_utils::draw_property("Physics Sphere Collisions Color", physics_sphere_collisions_color, true))
            {
                renderer::debug_renderer::set_physics_sphere_collisions_color(physics_sphere_collisions_color);
            }

            bool enable_physics_sphere_collisions = debug_renderer_data.enable_physics_sphere_collisions;
            if (ui_utils::draw_property("Enable Physics Sphere Collisions", enable_physics_sphere_collisions))
            {
                renderer::debug_renderer::set_physics_sphere_collisions_enabled(enable_physics_sphere_collisions);
            }

            ImGui::End();
        }
    }

    bool debug_panel::get_show_panel()
    {
        return s_show;
    }

    void debug_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
