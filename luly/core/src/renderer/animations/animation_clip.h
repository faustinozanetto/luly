#pragma once

#include "animation_bone.h"
#include "renderer/models/model.h"

struct aiNode;

namespace luly::renderer
{
    struct animation_clip_node
    {
        glm::mat4 transformation;
        std::string name;
        int children_count;
        std::vector<animation_clip_node> children;
    };

    class animation_clip
    {
    public:
        animation_clip(const std::string& animation_path, const std::shared_ptr<model>& model);
        ~animation_clip();

        /* Getters */
        const std::map<std::string, model_bone_info>& get_model_bones_info() const { return m_bones_info; }
        int get_ticks_per_second() const { return m_ticks_per_second; }
        float get_duration() const { return m_duration; }
        const animation_clip_node& get_root_node() const { return m_root_node; }

        animation_bone* find_bone(const std::string& name);

    private:
        void parse_hierarchy(animation_clip_node& dest, const aiNode* src);
        void parse_missing_bones(const aiAnimation* assimp_animation, const std::shared_ptr<model>& model);

        float m_duration;
        int m_ticks_per_second;
        std::vector<animation_bone> m_bones;
        animation_clip_node m_root_node;
        std::map<std::string, model_bone_info> m_bones_info;
    };
}
