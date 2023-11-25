#include "lypch.h"
#include "texture_cubemap.h"

#include <logging/log.h>
#include <utils/assert.h>

#include "texture_utils.h"

namespace luly::renderer
{
    texture_cubemap::texture_cubemap(const texture_specification& texture_specification)
    {
        m_texture_specification = texture_specification;

        calculate_mip_maps();

        LY_TRACE("Texture specification:");
        LY_TRACE("  - Width: {0}px", m_texture_specification.width);
        LY_TRACE("  - Height: {0}px", m_texture_specification.height);
        LY_TRACE("  - Channels: {0}", m_texture_specification.channels);
        LY_TRACE("  - Mipmap Levels: {0}", m_texture_specification.mip_map_levels);

        texture_cubemap::initialize();
    }

    void texture_cubemap::initialize()
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_handle);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle);

        uint32_t internal_format = texture_utils::get_texture_internal_format_to_opengl(
            m_texture_specification.internal_format);
        uint32_t format = texture_utils::get_texture_format_to_opengl(
            texture_utils::get_texture_format_from_internal_format(m_texture_specification.internal_format));

        for (unsigned int i = 0; i < 6; ++i)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internal_format, m_texture_specification.width,
                         m_texture_specification.height, 0, format,
                         GL_FLOAT, nullptr);
        }

        set_wrapping(texture_wrapping_type::wrap_s, texture_wrapping::clamp_to_edge);
        set_wrapping(texture_wrapping_type::wrap_t, texture_wrapping::clamp_to_edge);
        set_wrapping(texture_wrapping_type::wrap_r, texture_wrapping::clamp_to_edge);

        set_filtering(texture_filtering_type::filter_min, texture_filtering::linear);
        set_filtering(texture_filtering_type::filter_mag, texture_filtering::linear);
    }
}
