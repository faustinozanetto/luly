#include "lypch.h"
#include "vertex_buffer_object.h"

namespace luly::renderer
{
    vertex_buffer_object::vertex_buffer_object()
    {
        glGenBuffers(1, &m_handle);
    }

    vertex_buffer_object::~vertex_buffer_object()
    {
        glDeleteBuffers(1, &m_handle);
    }

    void vertex_buffer_object::bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_handle);
    }

    void vertex_buffer_object::un_bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void vertex_buffer_object::set_attribute(uint32_t index, int size, uint32_t type, int stride, bool normalized,
                                             const void* data)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, data);
    }

    void vertex_buffer_object::set_data(vertex_buffer_object_usage buffer_usage, int size, const void* data)
    {
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, get_buffer_usage_to_opengl(buffer_usage));
    }


    void vertex_buffer_object::set_layout_descriptor(
        const std::shared_ptr<vertex_buffer_layout_descriptor>& layout_descriptor)
    {
        m_layout_descriptor = layout_descriptor;
    }

    uint32_t vertex_buffer_object::get_buffer_usage_to_opengl(vertex_buffer_object_usage buffer_usage)
    {
        switch (buffer_usage)
        {
        case vertex_buffer_object_usage::static_draw:
            return GL_STATIC_DRAW;
        case vertex_buffer_object_usage::static_read:
            return GL_STATIC_READ;
        case vertex_buffer_object_usage::static_copy:
            return GL_STATIC_COPY;
        case vertex_buffer_object_usage::dynamic_draw:
            return GL_DYNAMIC_DRAW;
        case vertex_buffer_object_usage::dynamic_read:
            return GL_DYNAMIC_READ;
        case vertex_buffer_object_usage::dynamic_copy:
            return GL_DYNAMIC_COPY;
        }
        LY_ASSERT_MSG(false, "Invalid vertex buffer object usage!");
        return -1;
    }
}
