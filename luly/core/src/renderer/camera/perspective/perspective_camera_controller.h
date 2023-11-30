#pragma once

#include "perspective_camera.h"

#include <memory>

namespace luly::renderer
{
    enum class camera_keyboard_direction
    {
        forward,
        backward,
        left,
        right,
        up,
        down,
    };

    class perspective_camera_controller
    {
    public:
        perspective_camera_controller(const std::shared_ptr<perspective_camera>& perspective_camera);

        /* Methods */
        void on_update(float delta_time);
        void process_keyboard_input(float delta_time) const;
        void process_mouse_input(float x_offset, float y_offset, float delta_time);

    private:
        std::shared_ptr<perspective_camera> m_perspective_camera;

        glm::vec2 m_last_mouse_pos;
        float m_movement_speed;
        float m_mouse_speed;
    };
}
