﻿#include "lypch.h"
#include "texture_3d.h"

#include "texture_utils.h"

#include <stb_image.h>

namespace luly::renderer
{
    texture_3d::texture_3d(const texture_specification& texture_specification)
    {
        m_texture_specification = texture_specification;

        calculate_mip_maps();

        LY_TRACE("Texture specification:");
        LY_TRACE("  - Width: {0}px", m_texture_specification.width);
        LY_TRACE("  - Height: {0}px", m_texture_specification.height);
        LY_TRACE("  - Channels: {0}", m_texture_specification.channels);
        LY_TRACE("  - Mipmap Levels: {0}", m_texture_specification.mip_map_levels);

        texture_3d::initialize();
    }

    void texture_3d::initialize()
    {
        // Create OpenGL texture
        glCreateTextures(GL_TEXTURE_3D, 1, &m_handle);
        glBindTexture(GL_TEXTURE_3D, m_handle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage3D(m_handle, 1,
                           texture_utils::get_texture_internal_format_to_opengl(
                               m_texture_specification.internal_format),
                           m_texture_specification.width,
                           m_texture_specification.height, m_texture_specification.width);

        // Filtering
        set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
        set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

        // Wrapping
        set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
        set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);
        set_wrapping(texture_wrapping_type::wrap_r, texture_wrapping::clamp_to_edge);

        // Allocating Memory.
        if (m_texture_specification.data != nullptr)
        {
            const texture_format format = texture_utils::get_texture_format_from_internal_format(
                m_texture_specification.internal_format);
            glTextureSubImage3D(m_handle, 0, 0, 0, 0, m_texture_specification.width, m_texture_specification.height,
                                m_texture_specification.width,
                                texture_utils::get_texture_format_to_opengl(format),
                                m_texture_specification.internal_format == texture_internal_format::r11g11b10
                                    ? GL_FLOAT
                                    : GL_UNSIGNED_BYTE,
                                m_texture_specification.data);
            //    stbi_image_free(m_texture_specification.data);
        }
    }
}
