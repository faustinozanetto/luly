﻿#include "lypch.h"
#include "frame_buffer.h"

#include "renderer/textures/texture_utils.h"

#include "renderer/render_buffer/render_buffer_utils.h"

namespace luly::renderer
{
    frame_buffer::frame_buffer(int width, int height) : frame_buffer(width, height, {}, {})
    {
        LY_TRACE("Started creating frame buffer...");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = false;

        m_attachments = {};
        m_attachments_data = {};
        m_depth_attachment = {};
        m_depth_attachment_data = {};

        LY_TRACE("  - Width: {0} px", m_width);
        LY_TRACE("  - Height: {0} px", m_height);
        LY_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        LY_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        pre_initialize();

        LY_TRACE("Frame buffer created successfully.");
    }

    frame_buffer::frame_buffer(int width, int height, const std::vector<frame_buffer_attachment>& attachments)
    {
        LY_TRACE("Started creating frame buffer...");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = false;

        m_attachments_data = attachments;
        m_attachments = {};
        m_depth_attachment = {};
        m_depth_attachment_data = {};

        LY_TRACE("  - Width: {0} px", m_width);
        LY_TRACE("  - Height: {0} px", m_height);
        LY_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        LY_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        pre_initialize();

        LY_TRACE("Frame buffer created successfully.");
    }

    frame_buffer::frame_buffer(int width, int height, const std::vector<frame_buffer_attachment>& attachments,
                               const frame_buffer_attachment& depth_attachment)
    {
        LY_TRACE("Started creating frame buffer...");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = true;

        m_attachments_data = attachments;
        m_depth_attachment_data = depth_attachment;
        m_attachments = {};
        m_depth_attachment = {};

        LY_TRACE("  - Width: {0} px", m_width);
        LY_TRACE("  - Height: {0} px", m_height);
        LY_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        LY_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        pre_initialize();

        LY_TRACE("Frame buffer created successfully.");
    }

    frame_buffer::frame_buffer(int width, int height, const frame_buffer_attachment& depth_attachment)
    {
        LY_TRACE("Started creating frame buffer...");
        m_width = width;
        m_height = height;
        m_has_depth_attachment = true;

        m_attachments_data = {};
        m_depth_attachment_data = depth_attachment;
        m_attachments = {};
        m_depth_attachment = {};

        LY_TRACE("  - Width: {0} px", m_width);
        LY_TRACE("  - Height: {0} px", m_height);
        LY_TRACE("  - Attachments Count: {0}", m_attachments_data.size());
        LY_TRACE("  - Has Depth Attachment: '{0}'", m_has_depth_attachment ? "true" : "false");

        pre_initialize();

        LY_TRACE("Frame buffer created successfully.");
    }

    frame_buffer::~frame_buffer()
    {
        glDeleteFramebuffers(1, &m_handle);
        glDeleteTextures(m_attachments.size(), m_attachments.data());
        if (m_has_depth_attachment)
            glDeleteTextures(1, &m_depth_attachment);
    }

    void frame_buffer::bind()
    {
        LY_PROFILE_FUNCTION;
        glBindFramebuffer(GL_FRAMEBUFFER, m_handle);
    }

    void frame_buffer::un_bind()
    {
        LY_PROFILE_FUNCTION;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void frame_buffer::initialize()
    {
        LY_PROFILE_FUNCTION;
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
        const GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        LY_ASSERT_MSG(fboStatus == GL_FRAMEBUFFER_COMPLETE,
                      "An error occurred while creating frame buffer: " + std::to_string(fboStatus))

        un_bind();
    }

    void frame_buffer::resize(const glm::ivec2& dimensions, bool resize_attachments)
    {
        LY_PROFILE_FUNCTION;
        m_width = dimensions.x;
        m_height = dimensions.y;

        // If we mark resize attachments as true, we need
        // to set the new dimension to the attachments data array.
        if (resize_attachments)
        {
            for (frame_buffer_attachment& attachment : m_attachments_data)
            {
                attachment.size = dimensions;
            }
            m_depth_attachment_data.size = dimensions;
        }

        pre_initialize();
        initialize();
    }

    int frame_buffer::read_pixel(uint32_t attachment_index, int x, int y) const
    {
        LY_ASSERT_MSG(attachment_index < m_attachments.size(), "Invalid attachment index!")
        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);

        int pixel_data;
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);

        return pixel_data;
    }

    void frame_buffer::clear_attachment(uint32_t attachment_index, uint32_t format, uint32_t type,
                                        const void* value) const
    {
        LY_ASSERT_MSG(attachment_index < m_attachments.size(), "Invalid attachment index!")

        glClearTexImage(m_attachments[attachment_index], 0, format, type, value);
    }

    void frame_buffer::attach_texture(const std::shared_ptr<texture>& texture, uint32_t target,
                                      render_buffer_attachment_type attachment, uint32_t texture_target,
                                      bool register_attachment, int mipmaps_level)
    {
        LY_PROFILE_FUNCTION;
        glFramebufferTexture2D(target, render_buffer_utils::get_render_buffer_attachment_type_to_opengl(attachment),
                               texture_target, texture->get_handle_id(), mipmaps_level);
        if (register_attachment)
        {
            frame_buffer_attachment attachment_data;
            attachment_data.size = {texture->get_width(), texture->get_height()};
            attachment_data.filtering = texture_filtering::linear;
            attachment_data.wrapping = texture_wrapping::clamp_to_edge;
            attachment_data.internal_format = texture->get_internal_format();
            m_attachments_data.push_back(attachment_data);
            m_attachments.push_back(texture->get_handle_id());
        }
    }

    void frame_buffer::attach_depth_texture(uint32_t texture, int mipmaps_level)
    {
        LY_PROFILE_FUNCTION;
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, mipmaps_level);
        m_has_depth_attachment = true;
    }

    void frame_buffer::pre_initialize()
    {
        LY_PROFILE_FUNCTION;
        if (m_handle)
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
    }

    void frame_buffer::attach_color_texture(const frame_buffer_attachment& attachment, uint32_t handle_id,
                                            unsigned int index) const
    {
        LY_PROFILE_FUNCTION;
        glBindTexture(GL_TEXTURE_2D, handle_id);

        const uint32_t format = texture_utils::get_texture_format_to_opengl(
            texture_utils::get_texture_format_from_internal_format(attachment.internal_format));
        const uint32_t internal_format = texture_utils::get_texture_internal_format_to_opengl(
            attachment.internal_format);

        glTexImage2D(GL_TEXTURE_2D, 0, internal_format,
                     attachment.size.x, attachment.size.y, 0, format, GL_UNSIGNED_BYTE, nullptr);

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
        LY_PROFILE_FUNCTION;
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
