#pragma once

#include "renderer/common/bindable_object.h"
#include "renderer/textures/texture.h"

#include <glm/glm.hpp>

namespace luly::renderer
{
    enum class render_buffer_attachment_type
    {
        color,
        depth,
        stencil,
        depth_stencil
    };

    class render_buffer : public bindable_object
    {
    public:
        render_buffer(int width, int height, texture_internal_format internal_format);
        ~render_buffer() override;

        /* Getters */
        uint32_t get_handle_id() const { return m_handle; }
        int get_width() const { return m_width; }
        int get_height() const { return m_height; }
        texture_internal_format get_internal_format() const { return m_internal_format; }

        /* Setters */
        void set_storage_parameters(int width, int height, texture_internal_format internal_format);

        /* Overrides */
        void bind() override;
        void un_bind() override;

        /* Methods */
        void attach_to_frame_buffer(render_buffer_attachment_type attachment_type);
        void resize(const glm::ivec2& dimensions);

    private:
        uint32_t m_handle;
        int m_width;
        int m_height;
        texture_internal_format m_internal_format;
    };
}
