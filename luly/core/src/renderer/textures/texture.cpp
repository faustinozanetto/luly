#include "lypch.h"
#include "texture.h"

#include "texture_utils.h"

#include <cmath>
#include <logging/log.h>

namespace luly::renderer
{
    texture_specification::texture_specification(int width, int height, int channels, void* data) : width(width),
        height(height),
        channels(channels), mip_map_levels(0),
        internal_format(texture_utils::get_texture_formats_from_channel_count(channels).internal_format), data(data)
    {
    }

    texture::texture(const texture_specification& texture_specification)
    {
        m_texture_specification = texture_specification;

        calculate_mip_maps();

        LY_TRACE("Texture specification:");
        LY_TRACE("  - Width: {0}px", m_texture_specification.width);
        LY_TRACE("  - Height: {0}px", m_texture_specification.height);
        LY_TRACE("  - Channels: {0}", m_texture_specification.channels);
        LY_TRACE("  - Mipmap Levels: {0}", m_texture_specification.mip_map_levels);

        initialize();
    }

    texture::~texture()
    {
        glDeleteTextures(1, &m_handle);
    }

    void texture::bind()
    {
    }

    void texture::un_bind()
    {
    }

    void texture::set_filtering(texture_filtering_type filtering_type, texture_filtering filtering) const
    {
        glTextureParameteri(m_handle, texture_utils::get_texture_filtering_type_to_opengl(filtering_type),
                            texture_utils::get_texture_filtering_to_opengl(filtering));
    }

    void texture::set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping) const
    {
        glTextureParameteri(m_handle, texture_utils::get_texture_wrapping_type_to_opengl(wrapping_type),
                            texture_utils::get_texture_wrapping_to_opengl(wrapping));
    }

    void texture::calculate_mip_maps()
    {
        m_texture_specification.mip_map_levels = static_cast<int>(floor(
            log2((std::min)(m_texture_specification.width, m_texture_specification.height))));
    }

    void texture::initialize()
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
        const texture_format format = texture_utils::get_texture_format_from_internal_format(
            m_texture_specification.internal_format);
        glTextureSubImage2D(m_handle, 0, 0, 0, m_texture_specification.width, m_texture_specification.height,
                            texture_utils::get_texture_format_to_opengl(format),
                            m_texture_specification.internal_format == texture_internal_format::r11g11b10
                                ? GL_FLOAT
                                : GL_UNSIGNED_BYTE,
                            m_texture_specification.data);
    }
}
