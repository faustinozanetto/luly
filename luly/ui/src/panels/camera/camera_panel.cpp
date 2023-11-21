#include "camera_panel.h"

#include "application/application.h"
#include "utils/ui_utils.h"

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
            if (auto& current_scene = core::application::get().get_scene_manager()->get_current_scene())
            {
                auto& camera_manager = current_scene->get_camera_manager();
                auto& camera = camera_manager->get_perspective_camera();

                glm::vec3 camera_position = camera->get_position();
                if (ui_utils::draw_property("Position", camera_position, -50.0f, 50.0f, 0.01f))
                {
                    camera->set_position(camera_position);
                }
                float camera_fov = camera->get_fov();
                if (ui_utils::draw_property("FOV", camera_fov, 0.5f, 179.0f))
                {
                    camera->set_fov(camera_fov);
                }
                float camera_near_plane = camera->get_near_clip();
                if (ui_utils::draw_property("Near Plane", camera_near_plane, 0.01f, 100.0f, 0.1f))
                {
                    camera->set_near_clip(camera_near_plane);
                }

                float camera_far_plane = camera->get_far_clip();
                if (ui_utils::draw_property("Far Plane", camera_far_plane, 1.0f, 5000.0f, 1.0f))
                {
                    camera->set_far_clip(camera_far_plane);
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
