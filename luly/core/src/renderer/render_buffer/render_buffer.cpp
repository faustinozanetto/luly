#include "lypch.h"
#include "render_buffer.h"

#include "render_buffer_utils.h"
#include "renderer/textures/texture_utils.h"

namespace luly::renderer
{
    render_buffer::render_buffer(int width, int height, texture_internal_format internal_format)
    {
        LY_TRACE("Started creating render buffer...");

        glGenRenderbuffers(1, &m_handle);
        set_storage_parameters(width, height, internal_format);

        LY_TRACE("  - Width: {0}px", m_width);
        LY_TRACE("  - Height: {0}px", m_height);
        LY_TRACE("  - Format: '{0}'", texture_utils::get_texture_internal_format_to_string(m_internal_format));

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        LY_TRACE("Renderer buffer created successfully.");
    }

    render_buffer::~render_buffer()
    {
        glDeleteRenderbuffers(1, &m_handle);
    }

    void render_buffer::set_storage_parameters(int width, int height, texture_internal_format internal_format)
    {
        LY_PROFILE_FUNCTION;
        m_width = width;
        m_height = height;
        m_internal_format = internal_format;
        glBindRenderbuffer(GL_RENDERBUFFER, m_handle);
        glRenderbufferStorage(GL_RENDERBUFFER, texture_utils::get_texture_internal_format_to_opengl(internal_format),
                              m_width,
                              m_height);
    }

    void render_buffer::bind()
    {
        LY_PROFILE_FUNCTION;
        glBindRenderbuffer(GL_RENDERBUFFER, m_handle);
    }

    void render_buffer::un_bind()
    {
        LY_PROFILE_FUNCTION;
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void render_buffer::attach_to_frame_buffer(render_buffer_attachment_type attachment_type)
    {
        LY_PROFILE_FUNCTION;
        glFramebufferRenderbuffer(
            GL_FRAMEBUFFER, render_buffer_utils::get_render_buffer_attachment_type_to_opengl(attachment_type),
            GL_RENDERBUFFER, m_handle);
    }

    void render_buffer::resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
    }
}
