#include "element_buffer_object.h"

namespace luly::renderer
{
    element_buffer_object::element_buffer_object(int count)
    {
        m_count = count;
        glGenBuffers(1, &m_handle);
    }

    element_buffer_object::~element_buffer_object()
    {
        glDeleteBuffers(1, &m_handle);
    }

    void element_buffer_object::bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
    }

    void element_buffer_object::un_bind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void element_buffer_object::set_data(int size, const void* data)
    {
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
    }
}
