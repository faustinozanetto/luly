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

    texture_specification::texture_specification(int width, int height, int channels, void* data,
                                                 texture_internal_format internal_format): width(width),
        height(height),
        channels(channels), mip_map_levels(0),
        internal_format(internal_format), data(data)
    {
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

}
