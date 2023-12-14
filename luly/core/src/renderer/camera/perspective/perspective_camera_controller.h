#pragma once

#include "perspective_camera.h"

#include <memory>

namespace luly::renderer
{
    class perspective_camera_controller
    {
    public:
        perspective_camera_controller(const std::shared_ptr<perspective_camera>& perspective_camera);

        /* Getters */
        const glm::vec2& get_last_mouse_pos() const { return m_last_mouse_pos; }
        float get_movement_speed() const { return m_movement_speed; }
        float get_mouse_speed() const { return m_mouse_speed; }

        /* Setters */
        void set_movement_speed(float movement_speed) { m_movement_speed = movement_speed; }
        void set_mouse_speed(float mouse_speed) { m_mouse_speed = mouse_speed; }

        /* Methods */
        void on_update(float delta_time);
        void process_keyboard_input(float delta_time) const;
        void process_mouse_input(float x_offset, float y_offset, float delta_time) const;

    private:
        std::shared_ptr<perspective_camera> m_perspective_camera;

        glm::vec2 m_last_mouse_pos;
        float m_movement_speed;
        float m_mouse_speed;
    };
}
