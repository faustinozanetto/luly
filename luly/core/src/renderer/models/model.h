#pragma once

#include "renderer/meshes/mesh.h"
#include "renderer/materials/material.h"

#include <vector>
#include <memory>

namespace luly::renderer
{
    struct model_bone_info
    {
        int id;
        glm::mat4 offset;
    };

    class model
    {
    public:
        model(const std::vector<std::shared_ptr<mesh>>& meshes,
              const std::unordered_map<std::string, std::shared_ptr<material>>& materials = {},
              const std::map<std::string, model_bone_info>& bones_info = {}, int bones_count = -1);
        ~model();

        /* Getters */
        const std::vector<std::shared_ptr<mesh>>& get_meshes() const { return m_meshes; }
        const std::unordered_map<std::string, std::shared_ptr<material>>& get_materials() const { return m_materials; }
        std::map<std::string, model_bone_info>& get_bones_info() { return m_bones_info; }
        int& get_bones_count() { return m_bones_count; }

    private:
        std::vector<std::shared_ptr<mesh>> m_meshes;
        std::unordered_map<std::string, std::shared_ptr<material>> m_materials;

        std::map<std::string, model_bone_info> m_bones_info;
        int m_bones_count;
    };
}
