#include "lypch.h"
#include "mesh.h"

namespace luly::renderer
{
    mesh::mesh(const std::string& name, const std::vector<mesh_vertex>& vertices,
               const std::vector<mesh_index>& indices, int material_index)
    {
        m_name = name;
        m_vertices = vertices;
        m_indices = indices;
        m_material_index = material_index;
        initialize_buffers();
    }

    mesh::~mesh()
    {
        m_vao.reset();
        m_vertices.clear();
    }

    void mesh::initialize_buffers()
    {
        LY_PROFILE_FUNCTION;
        const size_t vertex_buffer_size = m_vertices.size() * sizeof(mesh_vertex);
        const size_t index_buffer_size = m_indices.size() * sizeof(mesh_index);

        // Create VAO
        m_vao = std::make_shared<vertex_array_object>();

        // Create VBO
        const auto& vertices_vbo = std::make_shared<
            vertex_buffer_object>();

        // Create EBO
        const auto& elements_buffer = std::make_shared<
            element_buffer_object>(m_indices.size());

        // Set buffers data
        m_vao->bind();
        vertices_vbo->bind();
        vertices_vbo->set_data(vertex_buffer_object_usage::static_draw, vertex_buffer_size, m_vertices.data());

        elements_buffer->bind();
        elements_buffer->set_data(index_buffer_size, m_indices.data());

        // Setup VBO layout descriptor.
        std::initializer_list<vertex_buffer_layout_entry>
            layout_elements = {
                {"a_pos", vertex_buffer_entry_type::FLOAT3, false},
                {"a_tex_coord", vertex_buffer_entry_type::FLOAT2, false},
                {"a_normal", vertex_buffer_entry_type::FLOAT3, false},
                {"a_tangent", vertex_buffer_entry_type::FLOAT3, false},
                {"a_bi_tangent", vertex_buffer_entry_type::FLOAT3, false},
                {"a_bone_ids", vertex_buffer_entry_type::INT4, false},
                {"a_weights", vertex_buffer_entry_type::FLOAT4, false}
            };

        const auto& vertices_vbo_layout_descriptor = std::make_shared<
            vertex_buffer_layout_descriptor>(layout_elements);
        vertices_vbo->set_layout_descriptor(vertices_vbo_layout_descriptor);

        // Configure VAO.
        m_vao->add_vertex_buffer(vertices_vbo);
        m_vao->set_element_buffer(elements_buffer);
        m_vao->un_bind();
    }
}
