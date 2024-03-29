﻿#pragma once

#include "renderer/arrays/vertex_array_object.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace luly::renderer
{
#define MAX_BONE_INFLUENCE 4

    using mesh_index = uint32_t;

    struct mesh_vertex
    {
        glm::vec3 position;
        glm::vec2 tex_coords;
        glm::vec3 normals;
        glm::vec3 tangent;
        glm::vec3 bi_tangent;

        int bone_ids[MAX_BONE_INFLUENCE];
        float weights[MAX_BONE_INFLUENCE];
    };

    class mesh
    {
    public:
        mesh(const std::string& name, const std::vector<mesh_vertex>& vertices, const std::vector<mesh_index>& indices,
             int material_index = 0);
        ~mesh();

        /* Getters */
        const std::shared_ptr<vertex_array_object>& get_vao() const { return m_vao; }
        const std::vector<mesh_vertex>& get_vertices() const { return m_vertices; }
        const std::vector<mesh_index>& get_indices() const { return m_indices; }
        const std::string& get_name() const { return m_name; }
        int get_material_index() const { return m_material_index; }

        /* Setters */
        void set_material_index(int material_index) { m_material_index = material_index; }

    private:
        void initialize_buffers();

        std::string m_name;
        int m_material_index;

        std::shared_ptr<vertex_array_object> m_vao;
        std::vector<mesh_vertex> m_vertices;
        std::vector<mesh_index> m_indices;
    };
}
