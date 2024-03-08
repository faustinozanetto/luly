#include "lypch.h"
#include "model_factory.h"

#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/textures/texture_factory.h"

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

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
            aiProcessPreset_TargetRealtime_Fast);

        // Check for loading errors.
        if (!assimp_scene || assimp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimp_scene->mRootNode)
        {
            LY_ASSERT_MSG(false, "An error occurred while loading model from file.")
        }

        const std::string directory = file_path.substr(0, file_path.find_last_of('/'));

        model_load_context model_load_context;
        model_load_context.bones_count = 0;
        parse_assimp_node(assimp_scene, assimp_scene->mRootNode, model_load_context);

        // Process materials.
        /*
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
                const auto& model_material = std::make_shared<material>(material_specification);
                model_materials.push_back(model_material);
            }
        }

        // Create the model map for mesh name to material from the previously loaded materials from the assimp scene.
        std::unordered_map<std::string, std::shared_ptr<material>> materials;
        for (std::shared_ptr<mesh>& mesh : model_load_context.meshes)
        {
            const std::shared_ptr<material> mesh_material = model_materials[mesh->get_material_index()];
            materials.insert({mesh->get_name(), mesh_material});
        }*/
        std::unordered_map<std::string, std::shared_ptr<material>> materials;

        return std::make_shared<model>(model_load_context.meshes, materials, model_load_context.bones_info,
                                       model_load_context.bones_count);
    }

    void model_factory::parse_assimp_node(const aiScene* assimp_scene, const aiNode* assimp_node,
                                          model_load_context& model_load_context)
    {
        LY_PROFILE_FUNCTION;
        // Process all the node's meshes (if any)
        for (unsigned int i = 0; i < assimp_node->mNumMeshes; i++)
        {
            aiMesh* mesh = assimp_scene->mMeshes[assimp_node->mMeshes[i]];
            model_load_context.meshes.push_back(parse_assimp_mesh(assimp_scene, mesh, model_load_context));
        }
        // Then do the same for each of its children
        for (unsigned int i = 0; i < assimp_node->mNumChildren; i++)
        {
            parse_assimp_node(assimp_scene, assimp_node->mChildren[i], model_load_context);
        }
    }

    std::shared_ptr<mesh> model_factory::parse_assimp_mesh(const aiScene* assimp_scene, aiMesh* assimp_mesh,
                                                           model_load_context& model_load_context)
    {
        LY_PROFILE_FUNCTION;
        std::vector<mesh_vertex> vertices;
        std::vector<mesh_index> indices;

        // Process vertices.
        for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++)
        {
            mesh_vertex vertex = parse_vertex_data(assimp_mesh, i);
            vertices.push_back(vertex);
        }

        // Process indices.
        for (unsigned int i = 0; i < assimp_mesh->mNumFaces; i++)
        {
            aiFace face = assimp_mesh->mFaces[i];
            indices.emplace_back(face.mIndices[0]);
            indices.emplace_back(face.mIndices[1]);
            indices.emplace_back(face.mIndices[2]);
        }

        parse_bone_weights(assimp_mesh, vertices, model_load_context);

        LY_TRACE("Parsed model mesh: ");
        LY_TRACE("   - Name: {}", assimp_mesh->mName.C_Str());
        LY_TRACE("   - Vertices: {}", vertices.size());
        LY_TRACE("   - Indices: {}", indices.size());

        return std::make_shared<mesh>(assimp_mesh->mName.C_Str(), vertices, indices, assimp_mesh->mMaterialIndex);
    }

    mesh_vertex model_factory::parse_vertex_data(aiMesh* assimp_mesh, int index)
    {
        mesh_vertex vertex;
        // Position.
        vertex.position.x = assimp_mesh->mVertices[index].x;
        vertex.position.y = assimp_mesh->mVertices[index].y;
        vertex.position.z = assimp_mesh->mVertices[index].z;

        // Normals
        vertex.normals.x = assimp_mesh->mNormals[index].x;
        vertex.normals.y = assimp_mesh->mNormals[index].y;
        vertex.normals.z = assimp_mesh->mNormals[index].z;

        // Tex coords
        if (assimp_mesh->HasTextureCoords(0))
        {
            vertex.tex_coords = glm::vec2(assimp_mesh->mTextureCoords[0][index].x,
                                          assimp_mesh->mTextureCoords[0][index].y);
        }
        else
        {
            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
        }

        // Tangent
        glm::vec3 tangent = glm::vec3(0.0f);
        if (assimp_mesh->mTangents)
        {
            tangent.x = assimp_mesh->mTangents[index].x;
            tangent.y = assimp_mesh->mTangents[index].y;
            tangent.z = assimp_mesh->mTangents[index].z;
        }
        vertex.tangent = tangent;

        // Bi tangent
        glm::vec3 bi_tangent = glm::vec3(0.0f);
        if (assimp_mesh->mBitangents)
        {
            bi_tangent.x = assimp_mesh->mBitangents[index].x;
            bi_tangent.y = assimp_mesh->mBitangents[index].y;
            bi_tangent.z = assimp_mesh->mBitangents[index].z;
        }
        vertex.bi_tangent = bi_tangent;

        // Fill default bone data
        for (int j = 0; j < MAX_BONE_INFLUENCE; j++)
        {
            vertex.bone_ids[j] = -1;
            vertex.weights[j] = 0.0f;
        }

        return vertex;
    }

    void model_factory::parse_bone_weights(const aiMesh* assimp_mesh,
                                           std::vector<mesh_vertex>& vertices, model_load_context& model_load_context)
    {
        for (uint32_t i = 0; i < assimp_mesh->mNumBones; ++i)
        {
            int bone_id;
            std::string bone_name = assimp_mesh->mBones[i]->mName.C_Str();
            // Check if bone was not already loaded before.
            if (!model_load_context.bones_info.contains(bone_name))
            {
                model_bone_info model_bone_info;
                model_bone_info.id = model_load_context.bones_count;
                model_bone_info.offset = convert_assimp_mat_to_glm(assimp_mesh->mBones[i]->mOffsetMatrix);
                model_load_context.bones_info[bone_name] = model_bone_info;

                bone_id = model_load_context.bones_count;
                model_load_context.bones_count++;
            }
            else
            {
                bone_id = model_load_context.bones_info[bone_name].id;
            }

            LY_ASSERT_MSG(bone_id != -1, "Invalid bone ID!")
            const aiVertexWeight* weights = assimp_mesh->mBones[i]->mWeights;
            const int num_weights = assimp_mesh->mBones[i]->mNumWeights;

            for (int weight_index = 0; weight_index < num_weights; ++weight_index)
            {
                const int vertex_id = weights[weight_index].mVertexId;
                const float weight = weights[weight_index].mWeight;

                LY_ASSERT_MSG(vertex_id <= vertices.size(), "Invalid vertex ID!")

                for (int j = 0; j < MAX_BONE_INFLUENCE; ++j)
                {
                    if (vertices[vertex_id].bone_ids[j] < 0)
                    {
                        vertices[vertex_id].weights[j] = weight;
                        vertices[vertex_id].bone_ids[j] = bone_id;
                        break;
                    }
                }
            }
        }
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

    glm::mat4 model_factory::convert_assimp_mat_to_glm(const aiMatrix4x4& assimp_mat)
    {
        glm::mat4 to;
        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
        to[0][0] = assimp_mat.a1;
        to[1][0] = assimp_mat.a2;
        to[2][0] = assimp_mat.a3;
        to[3][0] = assimp_mat.a4;
        to[0][1] = assimp_mat.b1;
        to[1][1] = assimp_mat.b2;
        to[2][1] = assimp_mat.b3;
        to[3][1] = assimp_mat.b4;
        to[0][2] = assimp_mat.c1;
        to[1][2] = assimp_mat.c2;
        to[2][2] = assimp_mat.c3;
        to[3][2] = assimp_mat.c4;
        to[0][3] = assimp_mat.d1;
        to[1][3] = assimp_mat.d2;
        to[2][3] = assimp_mat.d3;
        to[3][3] = assimp_mat.d4;
        return to;
    }
}
