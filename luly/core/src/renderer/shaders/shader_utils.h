﻿#pragma once

#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    class shader_utils
    {
    public:
        static uint32_t get_shader_type_to_opengl(shader_type shader_type);
        static const char* get_shader_type_to_string(shader_type shader_type);
    };
}
