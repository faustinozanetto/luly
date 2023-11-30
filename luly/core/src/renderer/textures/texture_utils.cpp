#include "lypch.h"
#include "texture_utils.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace luly::renderer
{
    const char* texture_utils::get_texture_filtering_to_string(texture_filtering filtering)
    {
        switch (filtering)
        {
        case texture_filtering::none:
            return "none";
        case texture_filtering::nearest:
            return "nearest";
        case texture_filtering::linear:
            return "linear";
        case texture_filtering::nearest_mipmap_nearest:
            return "nearest_mipmap_nearest";
        case texture_filtering::linear_mipmap_nearest:
            return "linear_mipmap_nearest";
        case texture_filtering::nearest_mipmap_linear:
            return "nearest_mipmap_linear";
        case texture_filtering::linear_mipmap_linear:
            return "linear_mipmap_linear";
        }
        LY_ASSERT_MSG(false, "Invalid texture filtering!");
        return nullptr;
    }

    const char* texture_utils::get_texture_filtering_type_to_string(texture_filtering_type filtering_type)
    {
        switch (filtering_type)
        {
        case texture_filtering_type::filter_min:
            return "filter_min";
        case texture_filtering_type::filter_mag:
            return "filter_mag";
        }
        LY_ASSERT_MSG(false, "Invalid texture filtering type!");
        return nullptr;
    }

    const char* texture_utils::get_texture_wrapping_to_string(texture_wrapping wrapping)
    {
        switch (wrapping)
        {
        case texture_wrapping::none:
            return "none";
        case texture_wrapping::repeat:
            return "repeat";
        case texture_wrapping::mirrored_repeat:
            return "mirrored_repeat";
        case texture_wrapping::clamp_to_edge:
            return "clamp_to_edge";
        case texture_wrapping::clamp_to_border:
            return "clamp_to_border";
        }
        LY_ASSERT_MSG(false, "Invalid texture wrapping!");
        return nullptr;
    }

    const char* texture_utils::get_texture_wrapping_type_to_string(texture_wrapping_type wrapping_type)
    {
        switch (wrapping_type)
        {
        case texture_wrapping_type::wrap_r:
            return "wrap_r";
        case texture_wrapping_type::wrap_s:
            return "wrap_s";
        case texture_wrapping_type::wrap_t:
            return "wrap_t";
        }
        LY_ASSERT_MSG(false, "Invalid texture wrapping type!");
        return nullptr;
    }

    int texture_utils::get_texture_filtering_to_opengl(texture_filtering filtering)
    {
        switch (filtering)
        {
        case texture_filtering::none:
            return -1;
        case texture_filtering::nearest:
            return GL_NEAREST;
        case texture_filtering::linear:
            return GL_LINEAR;
        case texture_filtering::nearest_mipmap_nearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case texture_filtering::linear_mipmap_nearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case texture_filtering::nearest_mipmap_linear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case texture_filtering::linear_mipmap_linear:
            return GL_LINEAR_MIPMAP_LINEAR;
        }
        LY_ASSERT_MSG(false, "Invalid texture filtering!");
        return 0;
    }

    int texture_utils::get_texture_filtering_type_to_opengl(texture_filtering_type filtering_type)
    {
        switch (filtering_type)
        {
        case texture_filtering_type::filter_min:
            return GL_TEXTURE_MIN_FILTER;
        case texture_filtering_type::filter_mag:
            return GL_TEXTURE_MAG_FILTER;
        }
        LY_ASSERT_MSG(false, "Invalid texture filtering type!");
        return 0;
    }

    int texture_utils::get_texture_wrapping_to_opengl(texture_wrapping wrapping)
    {
        switch (wrapping)
        {
        case texture_wrapping::none:
            return -1;
        case texture_wrapping::repeat:
            return GL_REPEAT;
        case texture_wrapping::mirrored_repeat:
            return GL_MIRRORED_REPEAT;
        case texture_wrapping::clamp_to_edge:
            return GL_CLAMP_TO_EDGE;
        case texture_wrapping::clamp_to_border:
            return GL_CLAMP_TO_BORDER;
        }
        LY_ASSERT_MSG(false, "Invalid texture wrapping!");
        return 0;
    }

    int texture_utils::get_texture_wrapping_type_to_opengl(texture_wrapping_type wrapping_type)
    {
        switch (wrapping_type)
        {
        case texture_wrapping_type::wrap_r:
            return GL_TEXTURE_WRAP_R;
        case texture_wrapping_type::wrap_s:
            return GL_TEXTURE_WRAP_S;
        case texture_wrapping_type::wrap_t:
            return GL_TEXTURE_WRAP_T;
        }
        LY_ASSERT_MSG(false, "Invalid texture wrapping type!");
        return 0;
    }

    texture_format texture_utils::get_texture_format_from_internal_format(texture_internal_format internal_format)
    {
        switch (internal_format)
        {
        case texture_internal_format::r8:
            return texture_format::red;
        case texture_internal_format::r16:
            return texture_format::red;
        case texture_internal_format::r16f:
            return texture_format::red;
        case texture_internal_format::r32f:
            return texture_format::red;
        case texture_internal_format::rg8:
            return texture_format::rg;
        case texture_internal_format::rg16:
            return texture_format::rg;
        case texture_internal_format::rg16f:
            return texture_format::rg;
        case texture_internal_format::rg32f:
            return texture_format::rg;
        case texture_internal_format::rgb8:
            return texture_format::rgb;
        case texture_internal_format::rgb16:
            return texture_format::rgb;
        case texture_internal_format::rgb16f:
            return texture_format::rgb;
        case texture_internal_format::rgb32f:
            return texture_format::rgb;
        case texture_internal_format::rgba8:
            return texture_format::rgba;
        case texture_internal_format::rgba16:
            return texture_format::rgba;
        case texture_internal_format::rgba16f:
            return texture_format::rgba;
        case texture_internal_format::rgba32f:
            return texture_format::rgba;
        case texture_internal_format::r11g11b10:
            return texture_format::rgb;
        case texture_internal_format::depth_component16:
            return texture_format::red;
        case texture_internal_format::depth_component24:
            return texture_format::red;
        case texture_internal_format::depth_component32:
            return texture_format::red;
        case texture_internal_format::depth_component32f:
            return texture_format::red;
        case texture_internal_format::stencil_index8:
            return texture_format::red;
        default:
            return texture_format::rgba;
        }
        LY_ASSERT_MSG(false, "Invalid texture internal format!");
    }

    const char* texture_utils::get_texture_format_to_string(texture_format format)
    {
        switch (format)
        {
        case texture_format::rg:
            return "rg";
        case texture_format::rgb:
            return "rgb";
        case texture_format::rgba:
            return "rgba";
        case texture_format::bgr:
            return "bgr";
        case texture_format::bgra:
            return "bgra";
        case texture_format::red:
            return "red";
        case texture_format::green:
            return "green";
        case texture_format::blue:
            return "blue";
        case texture_format::alpha:
            return "alpha";
        }
        LY_ASSERT_MSG(false, "Invalid texture format!");
        return nullptr;
    }

    const char* texture_utils::get_texture_internal_format_to_string(texture_internal_format internal_format)
    {
        switch (internal_format)
        {
        case texture_internal_format::r8:
            return "r8";
        case texture_internal_format::r16:
            return "r16";
        case texture_internal_format::r16f:
            return "r16f";
        case texture_internal_format::r32f:
            return "r32f";
        case texture_internal_format::rg8:
            return "rg8";
        case texture_internal_format::rg16:
            return "rg16";
        case texture_internal_format::rg16f:
            return "rg16f";
        case texture_internal_format::rg32f:
            return "rg32f";
        case texture_internal_format::rgb8:
            return "rgb8";
        case texture_internal_format::rgb16:
            return "rgb16";
        case texture_internal_format::rgb16f:
            return "rgb16f";
        case texture_internal_format::rgb32f:
            return "rgb32f";
        case texture_internal_format::rgba8:
            return "rgba8";
        case texture_internal_format::rgba16:
            return "rgba16";
        case texture_internal_format::rgba16f:
            return "rgba16f";
        case texture_internal_format::rgba32f:
            return "rgba32f";
        case texture_internal_format::r11g11b10:
            return "r11g11b10";
        case texture_internal_format::depth_component16:
            return "depth_component16";
        case texture_internal_format::depth_component24:
            return "depth_component24";
        case texture_internal_format::depth_component32:
            return "depth_component32";
        case texture_internal_format::depth_component32f:
            return "depth_component32f";
        case texture_internal_format::stencil_index8:
            return "stencil_index8";
        }
        LY_ASSERT_MSG(false, "Invalid texture internal format!");
        return nullptr;
    }

    int texture_utils::get_texture_format_to_opengl(texture_format format)
    {
        switch (format)
        {
        case texture_format::rg:
            return GL_RG;
        case texture_format::rgb:
            return GL_RGB;
        case texture_format::rgba:
            return GL_RGBA;
        case texture_format::bgr:
            return GL_BGR;
        case texture_format::bgra:
            return GL_BGRA;
        case texture_format::red:
            return GL_RED;
        case texture_format::green:
            return GL_GREEN;
        case texture_format::blue:
            return GL_BLUE;
        case texture_format::alpha:
            return GL_ALPHA;
        }
        LY_ASSERT_MSG(false, "Invalid texture format!");
        return 0;
    }

    int texture_utils::get_texture_internal_format_to_opengl(texture_internal_format internal_format)
    {
        switch (internal_format)
        {
        case texture_internal_format::r8:
            return GL_R8;
        case texture_internal_format::r16:
            return GL_R16;
        case texture_internal_format::r16f:
            return GL_R16F;
        case texture_internal_format::r32f:
            return GL_R32F;
        case texture_internal_format::rg8:
            return GL_RG8;
        case texture_internal_format::rg16:
            return GL_RG16;
        case texture_internal_format::rg16f:
            return GL_RG16F;
        case texture_internal_format::rg32f:
            return GL_RG32F;
        case texture_internal_format::rgb8:
            return GL_RGB8;
        case texture_internal_format::rgb16:
            return GL_RGB16;
        case texture_internal_format::rgb16f:
            return GL_RGB16F;
        case texture_internal_format::rgb32f:
            return GL_RGB32F;
        case texture_internal_format::rgba8:
            return GL_RGBA8;
        case texture_internal_format::rgba16:
            return GL_RGBA16;
        case texture_internal_format::rgba16f:
            return GL_RGBA16F;
        case texture_internal_format::rgba32f:
            return GL_RGBA32F;
        case texture_internal_format::r11g11b10:
            return GL_R11F_G11F_B10F;
        case texture_internal_format::depth_component16:
            return GL_DEPTH_COMPONENT;
        case texture_internal_format::depth_component24:
            return GL_DEPTH_COMPONENT24;
        case texture_internal_format::depth_component32:
            return GL_DEPTH_COMPONENT32;
        case texture_internal_format::depth_component32f:
            return GL_DEPTH_COMPONENT32F;
        case texture_internal_format::stencil_index8:
            return GL_STENCIL_INDEX8;
        }
        LY_ASSERT_MSG(false, "Invalid texture internal format!");
        return 0;
    }

    texture_formats texture_utils::get_texture_formats_from_channel_count(int channel_count)
    {
        LY_ASSERT_MSG(channel_count > 0 && channel_count < 5, "Invalid texture channels count!");
        texture_formats formats = {};
        if (channel_count == 4)
        {
            formats.internal_format = texture_internal_format::rgba16f;
            formats.format = texture_format::rgba;
        }
        else if (channel_count == 3)
        {
            formats.internal_format = texture_internal_format::rgb16f;
            formats.format = texture_format::rgb;
        }
        else if (channel_count == 2)
        {
            formats.internal_format = texture_internal_format::rg16f;
            formats.format = texture_format::rg;
        }
        else if (channel_count == 1)
        {
            formats.internal_format = texture_internal_format::r16f;
            formats.format = texture_format::red;
        }
        return formats;
    }
}
