#pragma once

#include "buffers/vertex/vertex_buffer_object.h"
#include "buffers/elements/element_buffer_object.h"

#include <vector>
#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace luly::renderer
{
    class vertex_array_object : public bindable_object
    {
    public:
        vertex_array_object();
        ~vertex_array_object() override;

        /* Getters */
        uint32_t get_handle() const { return m_handle; }

        const std::vector<std::shared_ptr<vertex_buffer_object>>& get_vertex_buffers() const
        {
            return m_vertex_buffers;
        }

        const std::shared_ptr<element_buffer_object>& get_element_buffer() const { return m_element_buffer; }

        void bind() override;
        void un_bind() override;

        void add_vertex_buffer(const std::shared_ptr<vertex_buffer_object>& vertex_buffer);
        void set_element_buffer(const std::shared_ptr<element_buffer_object>& element_buffer);

    private:
        uint32_t m_handle;
        uint32_t m_vbo_index;
        std::vector<std::shared_ptr<vertex_buffer_object>> m_vertex_buffers;
        std::shared_ptr<element_buffer_object> m_element_buffer;
    };
}
