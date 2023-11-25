#pragma once

#include "renderer/common/bindable_object.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace luly::renderer
{
    enum class texture_format
    {
        rg = 0,
        rgb,
        rgba,
        bgr,
        bgra,
        red,
        green,
        blue,
        alpha,
    };

    enum class texture_internal_format
    {
        r8 = 0,
        r16,
        r16f,
        r32f,
        rg8,
        rg16,
        rg16f,
        rg32f,
        rgb8,
        rgb16,
        rgb16f,
        rgb32f,
        rgba8,
        rgba16,
        rgba16f,
        rgba32f,
        r11g11b10,
        depth_component16,
        depth_component24,
        depth_component32,
        depth_component32f,
        stencil_index8
    };

    struct texture_formats
    {
        texture_format format;
        texture_internal_format internal_format;
    };

    enum class texture_filtering
    {
        none = 0,
        nearest,
        linear,
        nearest_mipmap_nearest,
        linear_mipmap_nearest,
        nearest_mipmap_linear,
        linear_mipmap_linear,
    };

    enum class texture_filtering_type
    {
        filter_min = 0,
        filter_mag,
    };

    enum class texture_wrapping
    {
        none = 0,
        repeat,
        mirrored_repeat,
        clamp_to_edge,
        clamp_to_border,
    };

    enum class texture_wrapping_type
    {
        wrap_s = 0,
        wrap_t,
        wrap_r,
    };

    struct texture_specification
    {
        int width;
        int height;
        int channels;
        int mip_map_levels;
        texture_internal_format internal_format;
        void* data;

        texture_specification() = default;
        texture_specification(int width, int height, int channels, void* data);
        texture_specification(int width, int height, int channels, void* data, texture_internal_format internal_format);
    };

    class texture : public bindable_object
    {
    public:
        ~texture() override;

        /* Getters */
        uint32_t get_handle_id() const { return m_handle; }
        int get_width() const { return m_texture_specification.width; }
        int get_height() const { return m_texture_specification.height; }
        int get_channels() const { return m_texture_specification.channels; }
        int get_mip_maps_levels() const { return m_texture_specification.mip_map_levels; }

        /* Overrides */
        void bind() override;
        void un_bind() override;

        /* Setters */
        void set_filtering(texture_filtering_type filtering_type, texture_filtering filtering) const;
        void set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping) const;

    protected:
        void calculate_mip_maps();
        virtual void initialize() = 0;

        uint32_t m_handle;
        texture_specification m_texture_specification;
    };
}
