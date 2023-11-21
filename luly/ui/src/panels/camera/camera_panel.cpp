#include "camera_panel.h"

#include "application/application.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    camera_panel::camera_panel() : ui_panel("camera_panel")
    {
    }

    camera_panel::~camera_panel()
    {
    }

    void camera_panel::on_render_panel()
    {
        auto& current_scene = core::application::get().get_scene_manager()->get_current_scene();

        if (ImGui::Begin("Camera"))
        {
            if (current_scene)
            {
                auto& camera_manager = current_scene->get_camera_manager();
                glm::vec3 camera_position = camera_manager->get_perspective_camera()->get_position();
                if (ui_utils::draw_property("Position", camera_position, -10.0f, 10.0f))
                {
                    camera_manager->get_perspective_camera()->set_position(camera_position);
                }
            }
            else
            {
                ImGui::Text("No current scene!");
            }
            ImGui::End();
        }
    }
}
