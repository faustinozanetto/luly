#include "lypch.h"
#include "animation_clip.h"

#include "renderer/models/model_factory.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace luly::renderer
{
    animation_clip::animation_clip(const std::string& animation_path, const std::shared_ptr<model>& model)
    {
        Assimp::Importer import;
        const aiScene* assimp_scene = import.ReadFile(
            animation_path,
            aiProcess_Triangulate);

        // Check for loading errors.
        if (!assimp_scene || assimp_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !assimp_scene->mRootNode)
        {
            LY_ASSERT_MSG(false, "An error occurred while loading animation from file.")
        }

        const aiAnimation* animation = assimp_scene->mAnimations[0];
        m_duration = animation->mDuration;
        m_ticks_per_second = animation->mTicksPerSecond;
        aiMatrix4x4 global_transformation = assimp_scene->mRootNode->mTransformation;
        global_transformation = global_transformation.Inverse();

        parse_hierarchy(m_root_node, assimp_scene->mRootNode);
        parse_missing_bones(animation, model);
    }

    animation_clip::~animation_clip()
    {
    }

    animation_bone* animation_clip::find_bone(const std::string& name)
    {
        const auto iter = std::ranges::find_if(m_bones,
                                               [&](const animation_bone& bone)
                                               {
                                                   return bone.get_name() == name;
                                               }
        );
        if (iter == m_bones.end()) return nullptr;

        return &(*iter);
    }

    void animation_clip::parse_hierarchy(animation_clip_node& dest, const aiNode* src)
    {
        dest.name = src->mName.data;
        dest.transformation = model_factory::convert_assimp_mat_to_glm(src->mTransformation);
        dest.children_count = src->mNumChildren;

        for (uint32_t i = 0; i < src->mNumChildren; i++)
        {
            animation_clip_node new_data;
            parse_hierarchy(new_data, src->mChildren[i]);
            dest.children.push_back(new_data);
        }
    }

    void animation_clip::parse_missing_bones(const aiAnimation* assimp_animation, const std::shared_ptr<model>& model)
    {
        const uint32_t size = assimp_animation->mNumChannels;

        std::map<std::string, model_bone_info>& model_bones_info = model->get_bones_info();
        int& model_bones_count = model->get_bones_count();

        //reading channels(bones engaged in an animation and their keyframes)
        for (int i = 0; i < size; i++)
        {
            const aiNodeAnim* channel = assimp_animation->mChannels[i];
            std::string bone_name = channel->mNodeName.data;

            if (!model_bones_info.contains(bone_name))
            {
                model_bones_info[bone_name].id = model_bones_count;
                model_bones_count++;
            }
            m_bones.emplace_back(channel->mNodeName.data,
                                 model_bones_info[channel->mNodeName.data].id, channel);
        }

        m_bones_info = model_bones_info;
    }
}
