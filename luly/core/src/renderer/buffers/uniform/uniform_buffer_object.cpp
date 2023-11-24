#include "lypch.h"
#include "uniform_buffer_object.h"

namespace luly::renderer
{
    uniform_buffer_object::uniform_buffer_object(uint32_t size, uint32_t binding)
    {
        glCreateBuffers(1, &m_handle_id);
        glNamedBufferData(m_handle_id, size, nullptr, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_handle_id);
    }

    uniform_buffer_object::~uniform_buffer_object()
    {
        glDeleteBuffers(1, &m_handle_id);
    }


    void uniform_buffer_object::bind(uint32_t slot) const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, slot, m_handle_id);
    }

    void uniform_buffer_object::bind()
    {
        bind(0);
    }

    void uniform_buffer_object::un_bind()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void uniform_buffer_object::set_data(const void* data, uint32_t size, uint32_t offset)
    {
        glNamedBufferSubData(m_handle_id, offset, size, data);
    }
}
