#include "lypch.h"
#include "shader_utils.h"

#include <utils/assert.h>
#include <logging/log.h>

namespace luly::renderer
{
    uint32_t shader_utils::get_shader_type_to_opengl(shader_type shader_type)
    {
        switch (shader_type)
        {
        case shader_type::fragment:
            return GL_FRAGMENT_SHADER;
        case shader_type::vertex:
            return GL_VERTEX_SHADER;
        case shader_type::geometry:
            return GL_GEOMETRY_SHADER;
        }
        LY_ASSERT_MSG(false, "Invalid shader type!");
        return 0;
    }

    const char* shader_utils::get_shader_type_to_string(shader_type shader_type)
    {
        switch (shader_type)
        {
        case shader_type::fragment:
            return "fragment";
        case shader_type::vertex:
            return "vertex";
        case shader_type::geometry:
            return "geometry";
        }
        LY_ASSERT_MSG(false, "Invalid shader type!");
        return nullptr;
    }
}
