#pragma once

#include "textures/texture.h"

#include <glm/glm.hpp>
#include <vector>

namespace luly::renderer
{
    struct frame_buffer_attachment
    {
        texture_internal_format internal_format;
        texture_filtering filtering;
        texture_wrapping wrapping;
        glm::vec2 size;
    };

    class frame_buffer : public bindable_object
    {
    public:
        frame_buffer(int width, int height);
        frame_buffer(int width, int height, const std::vector<frame_buffer_attachment>& attachments);
        frame_buffer(int width, int height, const std::vector<frame_buffer_attachment>& attachments,
                     const frame_buffer_attachment& depth_attachment);
        ~frame_buffer() override;

        /* Getters */
        uint32_t get_handle_id() const { return m_handle; }
        uint32_t get_depth_attachment() const { return m_depth_attachment; }
        uint32_t get_attachment_id(int index = 0) const { return m_attachments[index]; }
        int get_width() const { return m_width; }
        int get_height() const { return m_height; }

        /* Overrides */
        void bind() override;
        void un_bind() override;

    private:
        void initialize();
        void cleanup();

        void attach_color_texture(const frame_buffer_attachment& attachment, uint32_t handle_id, int index) const;
        void attach_depth_texture(const frame_buffer_attachment& attachment, uint32_t handle_id);

        uint32_t m_handle;
        int m_width;
        int m_height;
        bool m_has_depth_attachment;
        uint32_t m_depth_attachment;
        std::vector<uint32_t> m_attachments;
        frame_buffer_attachment m_depth_attachment_data;
        std::vector<frame_buffer_attachment> m_attachments_data;
    };
};
