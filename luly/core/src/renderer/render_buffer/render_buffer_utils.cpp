#include "lypch.h"
#include "render_buffer_utils.h"

namespace luly::renderer
{
    uint32_t render_buffer_utils::get_render_buffer_attachment_type_to_opengl(
        render_buffer_attachment_type attachment_type)
    {
        switch (attachment_type)
        {
        case render_buffer_attachment_type::color:
            return GL_COLOR_ATTACHMENT0;
        case render_buffer_attachment_type::depth:
            return GL_DEPTH_ATTACHMENT;
        case render_buffer_attachment_type::stencil:
            return GL_STENCIL_ATTACHMENT;
        case render_buffer_attachment_type::depth_stencil:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        }
        LY_ASSERT_MSG(false, "Invalid render buffer attachment type!");
        return 0;
    }

    const char* render_buffer_utils::get_render_buffer_attachment_type_to_string(
        render_buffer_attachment_type attachment_type)
    {
        switch (attachment_type)
        {
        case render_buffer_attachment_type::color:
            return "color";
        case render_buffer_attachment_type::depth:
            return "depth";
        case render_buffer_attachment_type::stencil:
            return "stencil";
        case render_buffer_attachment_type::depth_stencil:
            return "depth_stencil";
        }
        LY_ASSERT_MSG(false, "Invalid render buffer attachment type!");
        return nullptr;
    }
}
