#include "lypch.h"
#include "vertex_array_object.h"

#include <utils/assert.h>
#include <logging/log.h>

namespace luly::renderer
{
    vertex_array_object::vertex_array_object()
    {
        m_vbo_index = 0;
        glGenVertexArrays(1, &m_handle);
    }

    vertex_array_object::~vertex_array_object()
    {
        glDeleteVertexArrays(1, &m_handle);
    }

    void vertex_array_object::bind()
    {
        glBindVertexArray(m_handle);
    }

    void vertex_array_object::un_bind()
    {
        glBindVertexArray(0);
    }

    void vertex_array_object::add_vertex_buffer(const std::shared_ptr<vertex_buffer_object>& vertex_buffer)
    {
        bind();
        vertex_buffer->bind();

        const std::shared_ptr<vertex_buffer_layout_descriptor>& layout_descriptor = vertex_buffer->
            get_layout_descriptor();

        for (const vertex_buffer_layout_entry& descriptor_element : layout_descriptor->get_layout_elements())
        {
            switch (descriptor_element.get_type())
            {
            case vertex_buffer_entry_type::MAT3:
            case vertex_buffer_entry_type::MAT4:
                {
                    uint8_t count = vertex_buffer_layout_entry::get_vertex_buffer_entry_count(
                        descriptor_element.get_type());
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_vbo_index);
                        glVertexAttribPointer(m_vbo_index,
                                              count,
                                              vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(
                                                  descriptor_element.get_type()),
                                              descriptor_element.get_is_normalized() ? GL_TRUE : GL_FALSE,
                                              layout_descriptor->get_stride(),
                                              reinterpret_cast<const void*>(descriptor_element.get_offset() + sizeof(
                                                  float) * count * i));
                        glVertexAttribDivisor(m_vbo_index, 1);
                        m_vbo_index++;
                    }
                    break;
                }

            case vertex_buffer_entry_type::FLOAT:
            case vertex_buffer_entry_type::FLOAT2:
            case vertex_buffer_entry_type::FLOAT3:
            case vertex_buffer_entry_type::FLOAT4:
                {
                    glEnableVertexAttribArray(m_vbo_index);
                    glVertexAttribPointer(m_vbo_index,
                                          vertex_buffer_layout_entry::get_vertex_buffer_entry_count(
                                              descriptor_element.get_type()),
                                          vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(
                                              descriptor_element.get_type()),
                                          descriptor_element.get_is_normalized() ? GL_TRUE : GL_FALSE,
                                          layout_descriptor->get_stride(),
                                          reinterpret_cast<const void*>(descriptor_element.get_offset()));
                    m_vbo_index++;
                    break;
                }
            case vertex_buffer_entry_type::INT:
            case vertex_buffer_entry_type::INT2:
            case vertex_buffer_entry_type::INT3:
            case vertex_buffer_entry_type::INT4:
                {
                    glEnableVertexAttribArray(m_vbo_index);
                    glVertexAttribIPointer(m_vbo_index,
                                           vertex_buffer_layout_entry::get_vertex_buffer_entry_count(
                                               descriptor_element.get_type()),
                                           vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(
                                               descriptor_element.get_type()),
                                           layout_descriptor->get_stride(),
                                           reinterpret_cast<const void*>(descriptor_element.get_offset()));
                    m_vbo_index++;
                    break;
                }
            }
        }

        m_vertex_buffers.push_back(vertex_buffer);
    }

    void vertex_array_object::set_element_buffer(const std::shared_ptr<element_buffer_object>& element_buffer)
    {
        bind();
        element_buffer->bind();
        m_element_buffer = element_buffer;
    }
}
