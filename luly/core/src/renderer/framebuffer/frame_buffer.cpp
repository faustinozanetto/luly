#include "lypch.h"
#include "frame_buffer.h"

#include "renderer/textures/texture_utils.h"

#include <logging/log.h>
#include <utils/assert.h>

namespace luly::renderer
{
    frame_buffer::frame_buffer(int width, int height) : frame_buffer(width, height, {}, {})
    {
    }

    frame_buffer::frame_buffer(int width, int height, const std::vector<frame_buffer_attachment>& attachments)
    {
        LY_TRACE("Started creating frame buffer...");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = false;
        m_handle = 0;

        m_attachments = {};
        m_attachments_data = attachments;
        m_depth_attachment = {};

        LY_TRACE("  - Width: {0} px", m_width);
        LY_TRACE("  - Height: {0} px", m_height);
        LY_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        LY_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        initialize();

        LY_TRACE("Frame buffer created successfully.");
    }

    frame_buffer::frame_buffer(int width, int height, const std::vector<frame_buffer_attachment>& attachments,
                               const frame_buffer_attachment& depth_attachment)
    {
        LY_TRACE("Started creating frame buffer...");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = true;
        m_handle = 0;

        m_attachments = {};
        m_attachments_data = attachments;
        m_depth_attachment = {};
        m_depth_attachment_data = depth_attachment;

        LY_TRACE("  - Width: {0} px", m_width);
        LY_TRACE("  - Height: {0} px", m_height);
        LY_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        LY_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        initialize();

        LY_TRACE("Frame buffer created successfully.");
    }

    frame_buffer::~frame_buffer()
    {
        cleanup();
    }

    void frame_buffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    }

    void frame_buffer::un_bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void frame_buffer::initialize()
    {
        // If was previously created, cleanup data
        if (m_handle)
            cleanup();

        // Create fbo
        glGenFramebuffers(1, &m_handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);

        // Add color attachments
        if (!m_attachments_data.empty())
        {
            m_attachments.resize(m_attachments_data.size());
            for (unsigned int i = 0; i < m_attachments.size(); i++)
            {
                glGenTextures(1, &m_attachments[i]);
                attach_color_texture(m_attachments_data[i], m_attachments[i], i);
            }
        }

        // Add depth attachment
        if (m_has_depth_attachment)
        {
            glGenTextures(1, &m_depth_attachment);
            attach_depth_texture(m_depth_attachment_data, m_depth_attachment);
        }

        bind();

        // Draw buffers.
        if (!m_attachments.empty())
        {
            const GLenum buffers[8] = {
                GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3,
                GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7
            };
            glDrawBuffers(m_attachments.size(), buffers);
        }
        else if (m_attachments.empty() && m_has_depth_attachment)
        {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }

        // Error checking
        const auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        LY_ASSERT_MSG(fboStatus == GL_FRAMEBUFFER_COMPLETE,
                      "An error occurred while creating frame buffer: " + std::to_string(fboStatus))

        un_bind();
    }

    void frame_buffer::cleanup()
    {
        glDeleteFramebuffers(1, &m_handle);
        glDeleteTextures(m_attachments.size(), m_attachments.data());
        m_attachments.clear();
        if (m_has_depth_attachment)
        {
            glDeleteTextures(1, &m_depth_attachment);
            m_depth_attachment = 0;
        }
    }

    void frame_buffer::attach_color_texture(const frame_buffer_attachment& attachment, uint32_t handle_id,
                                            unsigned int index) const
    {
        glBindTexture(GL_TEXTURE_2D, handle_id);
        const texture_format format =
            texture_utils::get_texture_format_from_internal_format(attachment.internal_format);
        glTexImage2D(GL_TEXTURE_2D, 0, texture_utils::get_texture_internal_format_to_opengl(attachment.internal_format),
                     attachment.size.x, attachment.size.y, 0,
                     texture_utils::get_texture_format_to_opengl(format), GL_FLOAT, nullptr);

        // Filtering
        if (attachment.filtering != texture_filtering::none)
        {
            glTextureParameteri(
                handle_id, texture_utils::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_min),
                texture_utils::get_texture_filtering_to_opengl(attachment.filtering));
            glTextureParameteri(
                handle_id, texture_utils::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_mag),
                texture_utils::get_texture_filtering_to_opengl(attachment.filtering));
        }

        // Wrapping
        if (attachment.wrapping != texture_wrapping::none)
        {
            glTextureParameteri(
                handle_id, texture_utils::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_s),
                texture_utils::get_texture_wrapping_to_opengl(attachment.wrapping));
            glTextureParameteri(
                handle_id, texture_utils::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_t),
                texture_utils::get_texture_wrapping_to_opengl(attachment.wrapping));
        }

        // Attach texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, handle_id, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void frame_buffer::attach_depth_texture(const frame_buffer_attachment& attachment, uint32_t handle_id)
    {
        glBindTexture(GL_TEXTURE_2D, handle_id);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, attachment.size.x, attachment.size.y);

        // Filtering
        if (attachment.filtering != texture_filtering::none)
        {
            glTextureParameteri(
                handle_id, texture_utils::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_min),
                texture_utils::get_texture_filtering_to_opengl(attachment.filtering));
            glTextureParameteri(
                handle_id, texture_utils::get_texture_filtering_type_to_opengl(texture_filtering_type::filter_mag),
                texture_utils::get_texture_filtering_to_opengl(attachment.filtering));
        }

        // Wrapping
        if (attachment.wrapping != texture_wrapping::none)
        {
            glTextureParameteri(
                handle_id, texture_utils::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_s),
                texture_utils::get_texture_wrapping_to_opengl(attachment.wrapping));
            glTextureParameteri(
                handle_id, texture_utils::get_texture_wrapping_type_to_opengl(texture_wrapping_type::wrap_t),
                texture_utils::get_texture_wrapping_to_opengl(attachment.wrapping));
        }

        // Attach texture
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, handle_id, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
