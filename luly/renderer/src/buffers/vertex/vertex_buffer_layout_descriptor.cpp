#include "vertex_buffer_layout_descriptor.h"

#include <utils/assert.h>
#include <logging/log.h>

namespace luly::renderer
{
    vertex_buffer_layout_entry::vertex_buffer_layout_entry(const std::string& name, vertex_buffer_entry_type type,
                                                           bool is_normalized)
    {
        m_name = name;
        m_type = type;
        m_size = get_vertex_buffer_entry_type_size(type);
        m_offset = 0;
        m_normalized = is_normalized;
    }

    uint32_t vertex_buffer_layout_entry::get_vertex_buffer_entry_type_size(vertex_buffer_entry_type type)
    {
        switch (type)
        {
        case vertex_buffer_entry_type::FLOAT:
            return 4;
        case vertex_buffer_entry_type::FLOAT2:
            return 8;
        case vertex_buffer_entry_type::FLOAT3:
            return 12;
        case vertex_buffer_entry_type::FLOAT4:
            return 16;
        case vertex_buffer_entry_type::INT:
            return 4;
        case vertex_buffer_entry_type::INT2:
            return 8;
        case vertex_buffer_entry_type::INT3:
            return 12;
        case vertex_buffer_entry_type::INT4:
            return 16;
        case vertex_buffer_entry_type::MAT3:
            return 36;
        case vertex_buffer_entry_type::MAT4:
            return 64;
        case vertex_buffer_entry_type::BOOL:
            return 1;
        }
        LY_ASSERT_MSG(false, "Invalid vertex buffer entry type!");
        return 0;
    }

    uint32_t vertex_buffer_layout_entry::get_vertex_buffer_entry_count(vertex_buffer_entry_type type)
    {
        switch (type)
        {
        case vertex_buffer_entry_type::FLOAT:
            return 1;
        case vertex_buffer_entry_type::FLOAT2:
            return 2;
        case vertex_buffer_entry_type::FLOAT3:
            return 3;
        case vertex_buffer_entry_type::FLOAT4:
            return 4;
        case vertex_buffer_entry_type::INT:
            return 1;
        case vertex_buffer_entry_type::INT2:
            return 2;
        case vertex_buffer_entry_type::INT3:
            return 3;
        case vertex_buffer_entry_type::INT4:
            return 4;
        case vertex_buffer_entry_type::MAT3:
            return 3;
        case vertex_buffer_entry_type::MAT4:
            return 4;
        case vertex_buffer_entry_type::BOOL:
            return 1;
        }
        LY_ASSERT_MSG(false, "Invalid vertex buffer entry type!");
        return 0;
    }

    uint32_t vertex_buffer_layout_entry::get_vertex_buffer_entry_to_opengl(vertex_buffer_entry_type type)
    {
        switch (type)
        {
        case vertex_buffer_entry_type::FLOAT:
            return GL_FLOAT;
        case vertex_buffer_entry_type::FLOAT2:
            return GL_FLOAT;
        case vertex_buffer_entry_type::FLOAT3:
            return GL_FLOAT;
        case vertex_buffer_entry_type::FLOAT4:
            return GL_FLOAT;
        case vertex_buffer_entry_type::INT:
            return GL_INT;
        case vertex_buffer_entry_type::INT2:
            return GL_INT;
        case vertex_buffer_entry_type::INT3:
            return GL_INT;
        case vertex_buffer_entry_type::INT4:
            return GL_INT;
        case vertex_buffer_entry_type::MAT3:
            return GL_FLOAT;
        case vertex_buffer_entry_type::MAT4:
            return GL_FLOAT;
        case vertex_buffer_entry_type::BOOL:
            return GL_INT;
        }
        LY_ASSERT_MSG(false, "Invalid vertex buffer entry type!");
        return 0;
    }

    vertex_buffer_layout_descriptor::vertex_buffer_layout_descriptor(
        std::initializer_list<vertex_buffer_layout_entry> layout_elements)
    {
        m_layout_elements = layout_elements;

        size_t offset = 0;
        m_stride = 0;
        for (auto& descriptor_element : m_layout_elements)
        {
            descriptor_element.set_offset(offset);
            offset += descriptor_element.get_size();
            m_stride += descriptor_element.get_size();
        }
    }
}
