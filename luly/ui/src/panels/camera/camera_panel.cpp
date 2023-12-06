#include "camera_panel.h"

#include "utils/ui_utils.h"

#include <application/application.h>

namespace luly::ui
{
    bool camera_panel::s_show = true;

    camera_panel::camera_panel() : ui_panel("camera_panel")
    {
    }

    camera_panel::~camera_panel()
    {
    }

    void camera_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Camera", &s_show))
        {
            const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
            if (current_scene)
            {
                const std::shared_ptr<renderer::camera_manager>& camera_manager = current_scene->get_camera_manager();
                const std::shared_ptr<renderer::perspective_camera>& perspective_camera = camera_manager->
                    get_perspective_camera();

                ui_utils::draw_property("Camera Parameters");
                ImGui::Separator();
                glm::vec3 camera_position = perspective_camera->get_position();
                if (ui_utils::draw_property("Position", camera_position, -50.0f, 50.0f, 0.01f))
                {
                    perspective_camera->set_position(camera_position);
                }
                float camera_fov = perspective_camera->get_fov();
                if (ui_utils::draw_property("FOV", camera_fov, 0.5f, 179.0f))
                {
                    perspective_camera->set_fov(camera_fov);
                }
                float camera_near_plane = perspective_camera->get_near_clip();
                if (ui_utils::draw_property("Near Plane", camera_near_plane, 0.01f, 100.0f, 0.1f))
                {
                    perspective_camera->set_near_clip(camera_near_plane);
                }

                float camera_far_plane = perspective_camera->get_far_clip();
                if (ui_utils::draw_property("Far Plane", camera_far_plane, 1.0f, 5000.0f, 1.0f))
                {
                    perspective_camera->set_far_clip(camera_far_plane);
                }

                const std::shared_ptr<renderer::perspective_camera_controller>& perspective_camera_controller =
                    camera_manager->get_perspective_camera_controller();

                ui_utils::draw_property("Camera Controller Parameters");
                ImGui::Separator();
                float movement_speed = perspective_camera_controller->get_movement_speed();
                if (ui_utils::draw_property("Movement Speed", movement_speed, 0.1f, 30.0f, 0.01f))
                {
                    perspective_camera_controller->set_movement_speed(movement_speed);
                }

                float mouse_speed = perspective_camera_controller->get_mouse_speed();
                if (ui_utils::draw_property("Mouse Speed", mouse_speed, 0.1f, 30.0f, 0.01f))
                {
                    perspective_camera_controller->set_mouse_speed(mouse_speed);
                }
            }
            else
            {
                ImGui::Text("No current scene!");
            }
            ImGui::End();
        }
    }

    bool camera_panel::get_show_panel()
    {
        return s_show;
    }

    void camera_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
