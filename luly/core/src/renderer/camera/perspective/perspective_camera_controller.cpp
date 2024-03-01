#include "lypch.h"
#include "perspective_camera_controller.h"

#include "input/input_manager.h"

namespace luly::renderer
{
    perspective_camera_controller::perspective_camera_controller(
        const std::shared_ptr<perspective_camera>& perspective_camera)
    {
        m_perspective_camera = perspective_camera;
        m_movement_speed = 15.0f;
        m_mouse_speed = 15.0f;
        m_last_mouse_pos = glm::vec2(0, 0);
    }

    void perspective_camera_controller::on_update(float delta_time)
    {
        LY_PROFILE_FUNCTION;
        const glm::vec2 mouse_offset = input::input_manager::get_mouse_position() - m_last_mouse_pos;
        process_mouse_input(mouse_offset.x, mouse_offset.y, delta_time);
        m_last_mouse_pos = input::input_manager::get_mouse_position();

        process_keyboard_input(delta_time);

        m_perspective_camera->update_vectors();
        m_perspective_camera->update_view_matrix();
    }

    void perspective_camera_controller::process_keyboard_input(float delta_time) const
    {
        LY_PROFILE_FUNCTION;
        const float speed = m_movement_speed * delta_time;
        glm::vec3 updated_position = m_perspective_camera->get_position();

        if (input::input_manager::is_key_pressed(input::key::w))
        {
            const glm::vec3 movement = m_perspective_camera->get_front() * speed;
            updated_position += movement;
        }
        if (input::input_manager::is_key_pressed(input::key::s))
        {
            const glm::vec3 movement = m_perspective_camera->get_front() * speed;
            updated_position -= movement;
        }
        if (input::input_manager::is_key_pressed(input::key::a))
        {
            const glm::vec3 movement = m_perspective_camera->get_right() * speed;
            updated_position -= movement;
        }
        if (input::input_manager::is_key_pressed(input::key::d))
        {
            const glm::vec3 movement = m_perspective_camera->get_right() * speed;
            updated_position += movement;
        }

        m_perspective_camera->set_position(updated_position);
    }

    void perspective_camera_controller::process_mouse_input(float x_offset, float y_offset, float delta_time) const
    {
        LY_PROFILE_FUNCTION;
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
