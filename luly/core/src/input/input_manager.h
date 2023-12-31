﻿#pragma  once

#include <input/key_codes.h>
#include <input/mouse_codes.h>

#include <glm/glm.hpp>

namespace luly::input
{
    class input_manager
    {
    public:
        static bool is_key_pressed(key key_code);
        static bool is_mouse_button_pressed(mouse_button button_code);

        static glm::vec2 get_mouse_position();
    };
}
