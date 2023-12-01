#pragma once

#include "renderer/models/model.h"

#include <memory>
#include <vector>
#include <assimp/scene.h>

#include "renderer/textures/texture_2d.h"

namespace luly::renderer
{
    struct model_texture
    {
        std::string type;
        std::string path;
        std::shared_ptr<texture_2d> texture;
    };

    class model_factory
    {
    public:
        static std::shared_ptr<model> create_model_from_file(const std::string& file_path);
        static std::shared_ptr<model> create_model_from_meshes(const std::vector<std::shared_ptr<mesh>>& meshes);

    private:
        static std::shared_ptr<model> create_model_using_assimp(const std::string& file_path);
        static void parse_assimp_node(const aiScene* assimp_scene, const aiNode* assimp_node,
                                      std::vector<std::shared_ptr<mesh>>& model_meshes, const std::string& directory);
        static std::shared_ptr<mesh> parse_assimp_mesh(const aiScene* assimp_scene, aiMesh* assimp_mesh,
                                                       const std::string& directory);
        static std::vector<model_texture> parse_assimp_material_textures(
            aiMaterial* assimp_material, aiTextureType assimp_texture_type,
            const std::string& type_name, const std::string& directory, std::vector<model_texture>& loaded_textures);
    };
}
