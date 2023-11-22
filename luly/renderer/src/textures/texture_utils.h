#pragma once

#include "texture.h"

namespace luly::renderer
{
    class texture_utils
    {
    public:
        static const char* get_texture_filtering_to_string(texture_filtering filtering);
        static const char* get_texture_filtering_type_to_string(texture_filtering_type filtering_type);

        static const char* get_texture_wrapping_to_string(texture_wrapping wrapping);
        static const char* get_texture_wrapping_type_to_string(texture_wrapping_type wrapping_type);

        static int get_texture_filtering_to_opengl(texture_filtering filtering);
        static int get_texture_filtering_type_to_opengl(texture_filtering_type filtering_type);

        static int get_texture_wrapping_to_opengl(texture_wrapping wrapping);
        static int get_texture_wrapping_type_to_opengl(texture_wrapping_type wrapping_type);

        static texture_format get_texture_format_from_internal_format(texture_internal_format internal_format);

        static const char* get_texture_format_to_string(texture_format format);
        static const char* get_texture_internal_format_to_string(texture_internal_format internal_format);

        static int get_texture_format_to_opengl(texture_format format);
        static int get_texture_internal_format_to_opengl(texture_internal_format internal_format);

        static texture_formats get_texture_formats_from_channel_count(int channel_count);
    };
}
