#include "lypch.h"
#include "mesh_factory.h"

#include <glm/ext/scalar_constants.hpp>

namespace luly::renderer
{
    std::shared_ptr<mesh> mesh_factory::create_screen_quad_mesh()
    {
        LY_PROFILE_FUNCTION;
        std::vector<mesh_vertex> vertices = {
            {{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        };

        std::vector<mesh_index> indices = {
            0, 1, 2, 2, 3, 0
        };

        return std::make_shared<mesh>("mesh_screen_quad", vertices, indices);
    }

    std::shared_ptr<mesh> mesh_factory::create_cube_mesh()
    {
        LY_PROFILE_FUNCTION;
        std::vector<mesh_vertex> vertices = {
            // Front face
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},

            // Back face
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}},
        };

        std::vector<mesh_index> indices = {
            0, 1, 2, 2, 3, 0, // Front face
            4, 5, 6, 6, 7, 4, // Back face

            0, 3, 7, 7, 4, 0, // Left face
            1, 2, 6, 6, 5, 1, // Right face

            3, 2, 6, 6, 7, 3, // Top face
            0, 1, 5, 5, 4, 0 // Bottom face
        };

        return std::make_shared<mesh>("mesh_cube", vertices, indices);
    }

    std::shared_ptr<mesh> mesh_factory::create_sphere_mesh(int rings, int sectors, float radius)
    {
        LY_PROFILE_FUNCTION;
        std::vector<mesh_vertex> vertices;
        std::vector<mesh_index> indices;

        const float pi = glm::pi<float>();
        const float pi2 = pi * 2.0f;

        for (int i = 0; i <= rings; ++i)
        {
            float theta = i * pi / rings;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int j = 0; j <= sectors; ++j)
            {
                float phi = j * pi2 / sectors;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = cosPhi * sinTheta;
                float y = cosTheta;
                float z = sinPhi * sinTheta;

                float u = static_cast<float>(j) / static_cast<float>(sectors);
                float v = static_cast<float>(i) / static_cast<float>(rings);

                vertices.push_back({
                    glm::vec3(x, y, z) * radius,
                    glm::vec2(u, v),
                    glm::vec3(x, y, z), // normals can be the same as positions for a sphere
                    glm::vec3(1.0f, 0.0f, 0.0f), // tangent (arbitrary, can be calculated properly)
                    glm::vec3(0.0f, 1.0f, 0.0f) // bitangent (arbitrary, can be calculated properly)
                });
            }
        }

        for (int i = 0; i < rings; ++i)
        {
            for (int j = 0; j < sectors; ++j)
            {
                int nextRow = i + 1;
                int nextCol = j + 1;

                indices.push_back(i * (sectors + 1) + j);
                indices.push_back(nextRow * (sectors + 1) + j);
                indices.push_back(nextRow * (sectors + 1) + nextCol);

                indices.push_back(i * (sectors + 1) + j);
                indices.push_back(nextRow * (sectors + 1) + nextCol);
                indices.push_back(i * (sectors + 1) + nextCol);
            }
        }

        return std::make_shared<mesh>("mesh_sphere", vertices, indices);
    }
}
