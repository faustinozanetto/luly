#pragma once

#include "renderer/models/model.h"
#include "renderer/textures/texture_2d.h"

#include <memory>
#include <vector>
#include <assimp/scene.h>

namespace luly::renderer
{
    struct model_texture
    {
        std::string type;
        std::string path;
        std::shared_ptr<texture_2d> texture;
    };

    struct model_load_context
    {
        std::vector<std::shared_ptr<mesh>> meshes;
        std::map<std::string, model_bone_info> bones_info;
        int bones_count;
    };

    class model_factory
    {
    public:
        static std::shared_ptr<model> create_model_from_file(const std::string& file_path);
        static std::shared_ptr<model> create_model_from_meshes(const std::vector<std::shared_ptr<mesh>>& meshes);

        static glm::mat4 convert_assimp_mat_to_glm(const aiMatrix4x4& assimp_mat);

    private:
        static std::shared_ptr<model> create_model_using_assimp(const std::string& file_path);
        static void parse_assimp_node(const aiScene* assimp_scene, const aiNode* assimp_node,
                                      model_load_context& model_load_context);
        static std::shared_ptr<mesh> parse_assimp_mesh(const aiScene* assimp_scene, aiMesh* assimp_mesh,
                                                       model_load_context& model_load_context);
        static mesh_vertex parse_vertex_data(aiMesh* assimp_mesh, int index);
        static void parse_bone_weights(const aiMesh* assimp_mesh,
                                       std::vector<mesh_vertex>& vertices, model_load_context& model_load_context);
        static std::vector<model_texture> parse_assimp_material_textures(
            const aiMaterial* assimp_material, aiTextureType assimp_texture_type,
            const std::string& type_name, const std::string& directory, std::vector<model_texture>& loaded_textures);
    };
}
