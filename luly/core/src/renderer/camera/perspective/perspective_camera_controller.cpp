#include "lypch.h"
#include "perspective_camera_controller.h"

#include "input/input_manager.h"

namespace luly::renderer
{
    perspective_camera_controller::perspective_camera_controller(
        const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        m_perspective_camera = perspective_camera;
        m_movement_speed = 3.0f;
        m_mouse_speed = 6.0f;
        m_last_mouse_pos = glm::vec2(0, 0);
    }

    void perspective_camera_controller::on_update(float delta_time)
    {
        // Handle mouse input
        glm::vec2 mouse_offest = input::input_manager::get_mouse_position() - m_last_mouse_pos;
        process_mouse_input(mouse_offest.x, mouse_offest.y, delta_time);
        m_last_mouse_pos = input::input_manager::get_mouse_position();

        // Handle keyboard input
        process_keyboard_input(delta_time);

        m_perspective_camera->update_vectors();
        m_perspective_camera->update_view_matrix();
    }

    void perspective_camera_controller::process_keyboard_input(float delta_time) const
    {
        float speed = m_movement_speed * delta_time;
        glm::vec3 updated_position = m_perspective_camera->get_position();

        if (input::input_manager::is_key_pressed(input::key::w))
        {
            glm::vec3 movement = m_perspective_camera->get_front() * speed;
            updated_position += movement;
        }
        if (input::input_manager::is_key_pressed(input::key::s))
        {
            glm::vec3 movement = m_perspective_camera->get_front() * speed;
            updated_position -= movement;
        }
        if (input::input_manager::is_key_pressed(input::key::a))
        {
            glm::vec3 movement = m_perspective_camera->get_right() * speed;
            updated_position -= movement;
        }
        if (input::input_manager::is_key_pressed(input::key::d))
        {
            glm::vec3 movement = m_perspective_camera->get_right() * speed;
            updated_position += movement;
        }

        m_perspective_camera->set_position(updated_position);
    }

    void perspective_camera_controller::process_mouse_input(float x_offset, float y_offset, float delta_time)
    {
        if (input::input_manager::is_mouse_button_pressed(input::mouse_button::button_right))
        {
            x_offset *= m_mouse_speed;
            y_offset *= m_mouse_speed;

            float new_yaw = m_perspective_camera->get_yaw();
            new_yaw += x_offset * delta_time;

            float new_pitch = m_perspective_camera->get_pitch();
            new_pitch -= y_offset * delta_time;

            new_pitch = std::min(new_pitch, 89.0f);
            new_pitch = std::max(new_pitch, -89.0f);

            m_perspective_camera->set_yaw(new_yaw);
            m_perspective_camera->set_pitch(new_pitch);
        }
    }
}
