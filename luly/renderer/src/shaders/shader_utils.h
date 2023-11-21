#pragma once

#include "shaders/shader.h"
#include "renderer_api.h"

namespace luly::renderer
{
#define SHADER_TYPE_TOKEN "#shader"

    class LULY_RENDERER_API shader_utils
    {
    public:
        static uint32_t get_shader_type_to_opengl(shader_type shader_type);
        static const char* get_shader_type_to_string(shader_type shader_type);
    };
}
