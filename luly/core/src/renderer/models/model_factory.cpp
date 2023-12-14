#include "lypch.h"
#include "model_factory.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/textures/texture_factory.h"

namespace luly::renderer
{
    std::shared_ptr<model> model_factory::create_model_from_file(const std::string& file_path)
    {
        LY_PROFILE_FUNCTION;
        LY_TRACE("Started loading model from file...");
        LY_TRACE("  - File Path: '{0}'", file_path);

        const std::shared_ptr<model>& created_model = create_model_using_assimp(file_path);

        LY_TRACE("Model loaded from file successfully!");
        return created_model;
    }

    std::shared_ptr<model> model_factory::create_model_from_meshes(const std::vector<std::shared_ptr<mesh>>& meshes)
    {
        LY_PROFILE_FUNCTION;
        LY_TRACE("Started loading model from meshes...");

        const std::shared_ptr<model>& created_model = std::make_shared<model>(meshes);

        LY_TRACE("Model loaded from meshes successfully!");
        return created_model;
    }

    std::shared_ptr<model> model_factory::create_model_using_assimp(const std::string& file_path)
    {
        LY_PROFILE_FUNCTION;
        Assimp::Importer import;
        const aiScene* assimp_scene = import.ReadFile(
            file_path,
            aiProcessPreset_TargetRealtime_Fast | aiProcess_ConvertToLeftHanded);

        // Check for loading errors.
        if (!assimp_scene || assimp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimp_scene->mRootNode)
        {
            LY_ASSERT_MSG(false, "An error occurred while loading model from file.")
        }

        const std::string directory = file_path.substr(0, file_path.find_last_of('/'));

        std::vector<std::shared_ptr<mesh>> model_meshes;
        parse_assimp_node(assimp_scene, assimp_scene->mRootNode, model_meshes, directory);

        /*
        // Process materials.
        std::vector<std::shared_ptr<material>> model_materials;
        if (assimp_scene->HasMaterials())
        {
            for (unsigned int i = 0; i < assimp_scene->mNumMaterials; ++i)
            {
                aiMaterial* assimp_material = assimp_scene->mMaterials[i];

                std::vector<model_texture> loaded_textures;

                std::vector<model_texture> albedo_maps = parse_assimp_material_textures(
                    assimp_material, aiTextureType_DIFFUSE, "texture_diffuse", directory, loaded_textures);

                std::vector<model_texture> normal_maps = parse_assimp_material_textures(
                    assimp_material, aiTextureType_NORMALS, "texture_normal", directory, loaded_textures);

                std::vector<model_texture> roughness_maps = parse_assimp_material_textures(
                    assimp_material, aiTextureType_SPECULAR, "texture_roughness", directory, loaded_textures);

                std::vector<model_texture> metallic_maps = parse_assimp_material_textures(
                    assimp_material, aiTextureType_METALNESS, "texture_metallic", directory, loaded_textures);

                std::vector<model_texture> ao_maps = parse_assimp_material_textures(
                    assimp_material, aiTextureType_AMBIENT, "texture_ao", directory, loaded_textures);

                std::map<material_texture_type, material_texture> material_textures;
                if (!albedo_maps.empty())
                {
                    material_texture albedo_material_texture;
                    albedo_material_texture.texture = albedo_maps[0].texture;
                    albedo_material_texture.type = material_texture_type::albedo;
                    albedo_material_texture.is_enabled = true;
                    material_textures.insert({material_texture_type::albedo, albedo_material_texture});
                }
                if (!normal_maps.empty())
                {
                    material_texture normal_material_texture;
                    normal_material_texture.texture = normal_maps[0].texture;
                    normal_material_texture.type = material_texture_type::normal;
                    normal_material_texture.is_enabled = true;
                    material_textures.insert({material_texture_type::normal, normal_material_texture});
                }
                if (!roughness_maps.empty())
                {
                    material_texture roughness_material_texture;
                    roughness_material_texture.texture = roughness_maps[0].texture;
                    roughness_material_texture.type = material_texture_type::roughness;
                    roughness_material_texture.is_enabled = true;
                    material_textures.insert({material_texture_type::roughness, roughness_material_texture});
                }
                if (!metallic_maps.empty())
                {
                    material_texture metallic_material_texture;
                    metallic_material_texture.texture = metallic_maps[0].texture;
                    metallic_material_texture.type = material_texture_type::metallic;
                    metallic_material_texture.is_enabled = true;
                    material_textures.insert({material_texture_type::metallic, metallic_material_texture});
                }
                if (!ao_maps.empty())
                {
                    material_texture ao_material_texture;
                    ao_material_texture.texture = ao_maps[0].texture;
                    ao_material_texture.type = material_texture_type::ambient_occlusion;
                    ao_material_texture.is_enabled = true;
                    material_textures.insert({material_texture_type::ambient_occlusion, ao_material_texture});
                }

                const material_specification& material_specification =
                    std::make_shared<material_specification_builder>()->
                    with_textures(material_textures).build();
                const std::shared_ptr<material>& model_material = std::make_shared<material>(material_specification);
                model_materials.push_back(model_material);
            }
        }*/

        return std::make_shared<model>(model_meshes);
    }

    void model_factory::parse_assimp_node(const aiScene* assimp_scene, const aiNode* assimp_node,
                                          std::vector<std::shared_ptr<mesh>>& model_meshes,
                                          const std::string& directory)
    {
        LY_PROFILE_FUNCTION;
        // Process all the node's meshes (if any)
        for (unsigned int i = 0; i < assimp_node->mNumMeshes; i++)
        {
            aiMesh* mesh = assimp_scene->mMeshes[assimp_node->mMeshes[i]];
            model_meshes.push_back(parse_assimp_mesh(assimp_scene, mesh, directory));
        }
        // Then do the same for each of its children
        for (unsigned int i = 0; i < assimp_node->mNumChildren; i++)
        {
            parse_assimp_node(assimp_scene, assimp_node->mChildren[i], model_meshes, directory);
        }
    }

    std::shared_ptr<mesh> model_factory::parse_assimp_mesh(const aiScene* assimp_scene, aiMesh* assimp_mesh,
                                                           const std::string& directory)
    {
        LY_PROFILE_FUNCTION;
        std::vector<mesh_vertex> vertices;
        std::vector<mesh_index> indices;

        // Process vertices.
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
            if (assimp_mesh->mTextureCoords[0])
            {
                vertex.tex_coords = glm::vec2(assimp_mesh->mTextureCoords[0][i].x,
                                              1 - assimp_mesh->mTextureCoords[0][i].y);
            }
            else
            {
                vertex.tex_coords = glm::vec2(0.0f, 0.0f);
            }

            // Tangent
            vertex.tangent.x = assimp_mesh->mTangents[i].x;
            vertex.tangent.y = assimp_mesh->mTangents[i].y;
            vertex.tangent.z = assimp_mesh->mTangents[i].z;

            // Bi tangent
            vertex.bi_tangent.x = assimp_mesh->mBitangents[i].x;
            vertex.bi_tangent.y = assimp_mesh->mBitangents[i].y;
            vertex.bi_tangent.z = assimp_mesh->mBitangents[i].z;
            vertices.push_back(vertex);
        }

        // Process indices.
        for (unsigned int i = 0; i < assimp_mesh->mNumFaces; i++)
        {
            aiFace face = assimp_mesh->mFaces[i];
            // Retrieve all indices of the face and store them in the indices vector.
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        LY_TRACE("Parsed model mesh: ");
        LY_TRACE("   - Name: {}", assimp_mesh->mName.C_Str());
        LY_TRACE("   - Vertices: {}", vertices.size());
        LY_TRACE("   - Indices: {}", indices.size());

        return std::make_shared<mesh>(assimp_mesh->mName.C_Str(), vertices, indices, assimp_mesh->mMaterialIndex);
    }

    std::vector<model_texture> model_factory::parse_assimp_material_textures(const aiMaterial* assimp_material,
                                                                             aiTextureType assimp_texture_type,
                                                                             const std::string& type_name,
                                                                             const std::string& directory,
                                                                             std::vector<model_texture>&
                                                                             loaded_textures)
    {
        LY_PROFILE_FUNCTION;
        std::vector<model_texture> textures;
        for (unsigned int i = 0; i < assimp_material->GetTextureCount(assimp_texture_type); i++)
        {
            aiString str;
            assimp_material->GetTexture(assimp_texture_type, i, &str);

            bool skip = false;
            for (model_texture& loaded_texture : loaded_textures)
            {
                if (std::strcmp(loaded_texture.path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(loaded_texture);
                    skip = true;
                    break;
                }
            }
            if (!skip)
            {
                std::string filename = str.C_Str();
                filename = directory + '\\' + filename;

                model_texture model_texture;
                model_texture.type = type_name;
                model_texture.path = filename;
                model_texture.texture = texture_factory::create_texture_from_file(filename);
                textures.push_back(model_texture);
                loaded_textures.push_back(model_texture);
            }
        }
        return textures;
    }
}
