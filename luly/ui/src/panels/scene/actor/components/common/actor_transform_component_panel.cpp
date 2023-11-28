#include "actor_transform_component_panel.h"

#include "engine_ui.h"
#include "utils/ui_utils.h"

#include <scene/actor/components/transform_component.h>

namespace luly::ui
{
    actor_transform_component_panel::actor_transform_component_panel() : actor_component_panel("Transform Component")
    {
    }

    std::pair<bool, size_t> actor_transform_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<scene::transform_component>();
        auto component_hash = typeid(scene::transform_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_transform_component_panel::on_render_component_details()
    {
        const scene::transform_component& transform_component = engine_ui::get_ui_data().selected_actor->get_component<
            scene::transform_component>();

        const std::shared_ptr<math::transform>& transform = transform_component.get_transform();

        glm::vec3 location = glm::vec3(transform->get_location());
        if (ui_utils::draw_property("Position", location, -50.0f, 50.0f, 0.001f, 0))
        {
            transform->set_location(location);
        }

        glm::vec3 rotation = glm::vec3(glm::eulerAngles(transform->get_rotation()));
        if (ui_utils::draw_property("Rotation", rotation, -2 * glm::pi<float>(), 2 * glm::pi<float>(), 0.01f, 0))
        {
            const glm::quat pitch = glm::angleAxis(rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            const glm::quat yaw = glm::angleAxis(rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            const glm::quat roll = glm::angleAxis(rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

            transform->set_rotation(roll * yaw * pitch);
        }

        glm::vec3 scale = glm::vec3(transform->get_scale());
        if (ui_utils::draw_property("Scale", scale, -10.0f, 10.0f, 0.05f, 0))
        {
            transform->set_scale(scale);
        }
    }
}
