#include "physics_panel.h"

#include "physics/physics_world.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    bool physics_panel::s_show = true;

    physics_panel::physics_panel() : ui_panel("physics_panel")
    {
    }

    physics_panel::~physics_panel()
    {
    }

    void physics_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Physics", &s_show))
        {
            physics::physics_world& physics_world = physics::physics_world::get();
            /*
            glm::vec3 gravity = physics_world.get_gravity();
            if (ui_utils::draw_property("Gravity", gravity, -50.0f, 50.0f, 0.01f))
            {
                physics_world.set_gravity(gravity);
            }
*/
            bool simulate = physics_world.get_simulate();
            if (ui_utils::draw_property("Simulate", simulate))
            {
                physics_world.set_simulate(simulate);
            }
        }
        ImGui::End();
    }

    bool physics_panel::get_show_panel()
    {
        return s_show;
    }

    void physics_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
