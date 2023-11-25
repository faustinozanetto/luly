#include "lypch.h"
#include "texture_2d.h"

#include "texture_utils.h"

#include <logging/log.h>
#include <utils/assert.h>

#include "stb_image.h"

namespace luly::renderer
{
    texture_2d::texture_2d(const texture_specification& texture_specification)
    {
        m_texture_specification = texture_specification;

        calculate_mip_maps();

        LY_TRACE("Texture specification:");
        LY_TRACE("  - Width: {0}px", m_texture_specification.width);
        LY_TRACE("  - Height: {0}px", m_texture_specification.height);
        LY_TRACE("  - Channels: {0}", m_texture_specification.channels);
        LY_TRACE("  - Mipmap Levels: {0}", m_texture_specification.mip_map_levels);

        texture_2d::initialize();
    }

    void texture_2d::initialize()
    {
        // Create OpenGL texture
        glCreateTextures(GL_TEXTURE_2D, 1, &m_handle);
        glBindTexture(GL_TEXTURE_2D, m_handle);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTextureStorage2D(m_handle, 1,
                           texture_utils::get_texture_internal_format_to_opengl(
                               m_texture_specification.internal_format),
                           m_texture_specification.width,
                           m_texture_specification.height);

        // Filtering
        set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
        set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);

        // Wrapping
        set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
        set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);

        // Allocating Memory.
        if (m_texture_specification.data != nullptr)
        {
            const texture_format format = texture_utils::get_texture_format_from_internal_format(
                m_texture_specification.internal_format);
            glTextureSubImage2D(m_handle, 0, 0, 0, m_texture_specification.width, m_texture_specification.height,
                                texture_utils::get_texture_format_to_opengl(format),
                                m_texture_specification.internal_format == texture_internal_format::r11g11b10
                                    ? GL_FLOAT
                                    : GL_UNSIGNED_BYTE,
                                m_texture_specification.data);
            stbi_image_free(m_texture_specification.data);
        }
    }
}
