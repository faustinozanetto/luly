#pragma once

#include "renderer_api.h"

#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace luly::renderer
{
    enum class LULY_RENDERER_API vertex_buffer_entry_type
    {
        FLOAT,
        FLOAT2,
        FLOAT3,
        FLOAT4,
        INT,
        INT2,
        INT3,
        INT4,
        MAT3,
        MAT4,
        BOOL
    };

    class LULY_RENDERER_API vertex_buffer_layout_entry
    {
    public:
        vertex_buffer_layout_entry(const std::string& name, vertex_buffer_entry_type type, bool is_normalized);

        /* Getters */
        const std::string& get_name() const { return m_name; }
        vertex_buffer_entry_type get_type() const { return m_type; }
        uint32_t get_size() const { return m_size; }
        uint32_t get_offset() const { return m_offset; }
        bool get_is_normalized() const { return m_normalized; }

        /* Setters */
        void set_offset(uint32_t offset) { m_offset = offset; }

        static uint32_t get_vertex_buffer_entry_type_size(vertex_buffer_entry_type type);
        static uint32_t get_vertex_buffer_entry_count(vertex_buffer_entry_type type);
        static uint32_t get_vertex_buffer_entry_to_opengl(vertex_buffer_entry_type type);

    private:
        std::string m_name;
        vertex_buffer_entry_type m_type;
        uint32_t m_offset;
        uint32_t m_size;
        bool m_normalized;
    };

    class vertex_buffer_layout_descriptor
    {
    public:
        vertex_buffer_layout_descriptor(std::initializer_list<vertex_buffer_layout_entry> layout_elements);

        /* Getters */
        const std::vector<vertex_buffer_layout_entry>& get_layout_elements() const { return m_layout_elements; }
        uint32_t get_stride() const { return m_stride; }

    private:
        std::vector<vertex_buffer_layout_entry> m_layout_elements;
        uint32_t m_stride;
    };
}
