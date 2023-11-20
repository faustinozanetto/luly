#include "perspective_camera_controller.h"

#include "time/engine_time.h"

namespace luly::renderer
{
    perspective_camera_controller::perspective_camera_controller(
        const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        m_perspective_camera = perspective_camera;
        m_movement_speed = 15.0f;
    }

    void perspective_camera_controller::process_keyboard_input(camera_keyboard_direction direction) const
    {
        const float speed = m_movement_speed * time::get_delta_time();
        glm::vec3 updated_position = m_perspective_camera->get_position();

        if (direction == camera_keyboard_direction::forward)
        {
            updated_position += m_perspective_camera->get_front() * speed;
        }
        else if (direction == camera_keyboard_direction::backward)
        {
            updated_position -= m_perspective_camera->get_front() * speed;
        }
        else if (direction == camera_keyboard_direction::left)
        {
            updated_position -= m_perspective_camera->get_right() * speed;
        }
        else if (direction == camera_keyboard_direction::right)
        {
            updated_position += m_perspective_camera->get_right() * speed;
        }
        else if (direction == camera_keyboard_direction::up)
        {
            updated_position += m_perspective_camera->get_up() * speed;
        }
        else if (direction == camera_keyboard_direction::down)
        {
            updated_position -= m_perspective_camera->get_up() * speed;
        }
        
        m_perspective_camera->set_position(updated_position);
        m_perspective_camera->update_view_matrix();
    }
}
