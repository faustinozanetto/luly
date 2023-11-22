#pragma once

#include "common/bindable_object.h"
#include "vertex_buffer_layout_descriptor.h"

#include <memory>
#include <glad/glad.h>
#include <glfw/glfw3.h>

namespace luly::renderer
{
    enum class vertex_buffer_object_usage
    {
        static_draw,
        static_read,
        static_copy,
        dynamic_draw,
        dynamic_read,
        dynamic_copy
    };

    class vertex_buffer_object : public bindable_object
    {
    public:
        vertex_buffer_object();
        ~vertex_buffer_object() override;

        /* Getters*/
        uint32_t get_handle_id() const { return m_handle; }

        const std::shared_ptr<vertex_buffer_layout_descriptor>& get_layout_descriptor() const
        {
            return m_layout_descriptor;
        }

        void bind() override;
        void un_bind() override;

        void set_attribute(uint32_t index, int size, uint32_t type, int stride, bool normalized, const void* data);
        void set_data(vertex_buffer_object_usage buffer_usage, int size, const void* data);

        void set_layout_descriptor(const std::shared_ptr<vertex_buffer_layout_descriptor>& layout_descriptor);

        /* Utils */
        static uint32_t get_buffer_usage_to_opengl(vertex_buffer_object_usage buffer_usage);

    private:
        uint32_t m_handle;
        std::shared_ptr<vertex_buffer_layout_descriptor> m_layout_descriptor;
    };
}
