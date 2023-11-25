#pragma once

#include "render_buffer.h"

namespace luly::renderer
{
    class render_buffer_utils
    {
    public:
        static uint32_t get_render_buffer_attachment_type_to_opengl(render_buffer_attachment_type attachment_type);
        static const char* get_render_buffer_attachment_type_to_string(render_buffer_attachment_type attachment_type);
    };
}
