#pragma once

#include "common/bindable_object.h"
#include "renderer_api.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace luly::renderer
{
    enum class LULY_RENDERER_API texture_format
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

    enum class LULY_RENDERER_API texture_internal_format
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

    struct LULY_RENDERER_API texture_formats
    {
        texture_format format;
        texture_internal_format internal_format;
    };

    enum class LULY_RENDERER_API texture_filtering
    {
        none = 0,
        nearest,
        linear,
        nearest_mipmap_nearest,
        linear_mipmap_nearest,
        nearest_mipmap_linear,
        linear_mipmap_linear,
    };

    enum class LULY_RENDERER_API texture_filtering_type
    {
        filter_min = 0,
        filter_mag,
    };

    enum class LULY_RENDERER_API texture_wrapping
    {
        none = 0,
        repeat,
        mirrored_repeat,
        clamp_to_edge,
        clamp_to_border,
    };

    enum class LULY_RENDERER_API texture_wrapping_type
    {
        wrap_s = 0,
        wrap_t,
        wrap_r,
    };

    struct LULY_RENDERER_API texture_specification
    {
        int width;
        int height;
        int channels;
        int mip_map_levels;
        texture_internal_format internal_format;
        void* data;

        texture_specification() = default;
        texture_specification(int width, int height, int channels, void* data);
    };

    class LULY_RENDERER_API texture : public bindable_object
    {
    public:
        texture(const texture_specification& texture_specification);
        ~texture() override;

        /* Getters */
        uint32_t get_handle_id() const { return m_handle; }

        /* Overrides */
        void bind() override;
        void un_bind() override;

        /* Setters */
        void set_filtering(texture_filtering_type filtering_type, texture_filtering filtering) const;
        void set_wrapping(texture_wrapping_type wrapping_type, texture_wrapping wrapping) const;

    private:
        void calculate_mip_maps();
        void initialize();

        uint32_t m_handle;
        texture_specification m_texture_specification;
    };
}
