#pragma once

#include "models/model.h"

#include <memory>
#include <vector>
#include <assimp/scene.h>

namespace luly::renderer
{
    class model_factory
    {
    public:
        static std::shared_ptr<model> create_model_from_file(const std::string& file_path);
        static std::shared_ptr<model> create_model_from_meshes(const std::vector<std::shared_ptr<mesh>>& meshes);

    private:
        static std::shared_ptr<model> create_model_using_assimp(const std::string& file_path);
        static void parse_assimp_node(const aiScene* assimp_scene, const aiNode* assimp_node,
                                      std::vector<std::shared_ptr<mesh>>& model_meshes);
        static std::shared_ptr<mesh> parse_assimp_mesh(aiMesh* assimp_mesh);
    };
}
