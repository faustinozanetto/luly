#include "lypch.h"
#include "model_factory.h"

#include <logging/log.h>
#include <utils/assert.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

namespace luly::renderer
{
    std::shared_ptr<model> model_factory::create_model_from_file(const std::string& file_path)
    {
        LY_TRACE("Started loading model from file...");
        LY_TRACE("  - File Path: '{0}'", file_path);

        const auto& created_model = create_model_using_assimp(file_path);

        LY_TRACE("Model loaded from file successfully!");
        return created_model;
    }

    std::shared_ptr<model> model_factory::create_model_from_meshes(const std::vector<std::shared_ptr<mesh>>& meshes)
    {
        LY_TRACE("Started loading model from meshes...");

        const auto& created_model = std::make_shared<model>(meshes);

        LY_TRACE("Model loaded from meshes successfully!");
        return created_model;
    }

    std::shared_ptr<model> model_factory::create_model_using_assimp(const std::string& file_path)
    {
        Assimp::Importer import;
        const aiScene* assimp_scene = import.ReadFile(
            file_path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
            aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace | aiProcess_PreTransformVertices);

        // Check for loading errors.
        if (!assimp_scene || assimp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimp_scene->mRootNode)
        {
            LY_ASSERT_MSG(false, "An error occurred while loading model from file.")
        }

        std::vector<std::shared_ptr<mesh>> model_meshes;
        parse_assimp_node(assimp_scene, assimp_scene->mRootNode, model_meshes);

        return std::make_shared<model>(model_meshes);
    }

    void model_factory::parse_assimp_node(const aiScene* assimp_scene, const aiNode* assimp_node,
                                          std::vector<std::shared_ptr<mesh>>& model_meshes)
    {
        // Process all the node's meshes (if any)
        for (unsigned int i = 0; i < assimp_node->mNumMeshes; i++)
        {
            aiMesh* mesh = assimp_scene->mMeshes[assimp_node->mMeshes[i]];
            model_meshes.push_back(parse_assimp_mesh(mesh));
        }
        // Then do the same for each of its children
        for (unsigned int i = 0; i < assimp_node->mNumChildren; i++)
        {
            parse_assimp_node(assimp_scene, assimp_node->mChildren[i], model_meshes);
        }
    }

    std::shared_ptr<mesh> model_factory::parse_assimp_mesh(aiMesh* assimp_mesh)
    {
        std::vector<mesh_vertex> vertices;
        std::vector<mesh_index> indices;

        for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++)
        {
            mesh_vertex vertex;

            // Position.
            vertex.position.x = assimp_mesh->mVertices[i].x;
            vertex.position.y = assimp_mesh->mVertices[i].y;
            vertex.position.z = assimp_mesh->mVertices[i].z;

            // Normals
            vertex.normals.x = assimp_mesh->mNormals[i].x;
            vertex.normals.y = assimp_mesh->mNormals[i].y;
            vertex.normals.z = assimp_mesh->mNormals[i].z;

            // Tex coords
            if (assimp_mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
            {
                vertex.tex_coords = glm::vec2(assimp_mesh->mTextureCoords[0][i].x, assimp_mesh->mTextureCoords[0][i].y);
            }

            // Tangent
            if (assimp_mesh->mTangents)
            {
                vertex.tangent.x = assimp_mesh->mTangents[i].x;
                vertex.tangent.y = assimp_mesh->mTangents[i].y;
                vertex.tangent.z = assimp_mesh->mTangents[i].z;
            }

            // Bi tangent
            if (assimp_mesh->mBitangents)
            {
                vertex.bi_tangent.x = assimp_mesh->mBitangents[i].x;
                vertex.bi_tangent.y = assimp_mesh->mBitangents[i].y;
                vertex.bi_tangent.z = assimp_mesh->mBitangents[i].z;
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0; i < assimp_mesh->mNumFaces; i++)
        {
            aiFace face = assimp_mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector.
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        return std::make_shared<mesh>(assimp_mesh->mName.C_Str(), vertices, indices);
    }
}
