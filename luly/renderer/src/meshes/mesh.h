#pragma once

#include "renderer_api.h"
#include "arrays/vertex_array_object.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace luly::renderer
{
    typedef uint32_t mesh_index;

    struct LULY_RENDERER_API mesh_vertex
    {
        glm::vec3 position;
        glm::vec2 tex_coords;
        glm::vec3 normals;
        glm::vec3 tangent;
        glm::vec3 bi_tangent;
    };

    class LULY_RENDERER_API mesh
    {
    public:
        mesh(const std::string& name, const std::vector<mesh_vertex>& vertices, const std::vector<mesh_index>& indices);

        /* Getters */
        const std::shared_ptr<vertex_array_object>& get_vao() const { return m_vao; }
        const std::vector<mesh_vertex>& get_vertices() const { return m_vertices; }
        const std::vector<mesh_index>& get_indices() const { return m_indices; }
        const std::string& get_name() const { return m_name; }

    private:
        void initialize_buffers();

        std::string m_name;

        std::shared_ptr<vertex_array_object> m_vao;
        std::vector<mesh_vertex> m_vertices;
        std::vector<mesh_index> m_indices;
    };
}
