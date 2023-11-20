#pragma once
#include <memory>

#include "perspective_camera.h"

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
        void process_keyboard_input(camera_keyboard_direction direction) const;

    private:
        std::shared_ptr<perspective_camera> m_perspective_camera;
        float m_movement_speed;
    };
}
