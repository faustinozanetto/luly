#include "lypch.h"
#include "animation_controller.h"

namespace luly::renderer
{
    animation_controller::animation_controller(const std::shared_ptr<animation_clip>& animation_clip)
    {
        m_current_time = 0.0f;
        m_delta_time = 0.0f;
        m_speed_multiplier = 1.0f;
        m_play_animation = false;
        m_animation_clip = animation_clip;

        m_final_bone_matrices.reserve(100);
        for (int i = 0; i < 100; i++)
            m_final_bone_matrices.push_back(glm::mat4(1.0f));
    }

    animation_controller::~animation_controller()
    {
    }

    void animation_controller::on_update(float delta_time)
    {
        if (!m_play_animation) return;

        m_delta_time = delta_time;
        if (m_animation_clip)
        {
            m_current_time += m_animation_clip->get_ticks_per_second() * delta_time * m_speed_multiplier;
            m_current_time = fmod(m_current_time, m_animation_clip->get_duration());
            calculate_bone_transform(&m_animation_clip->get_root_node(), glm::mat4(1.0f));
        }
    }

    void animation_controller::play()
    {
        m_play_animation = true;
        m_current_time = 0.0f;
    }

    void animation_controller::pause()
    {
        m_play_animation = false;
    }

    void animation_controller::resume()
    {
        m_play_animation = true;
    }

    void animation_controller::stop()
    {
        m_play_animation = false;
        m_current_time = 0.0f;
    }

    void animation_controller::calculate_bone_transform(const animation_clip_node* animation_clip_node, const glm::mat4& parent_transform)
    {
        const std::string node_name = animation_clip_node->name;
        glm::mat4 node_transform = animation_clip_node->transformation;

        if (animation_bone* bone = m_animation_clip->find_bone(node_name))
        {
            bone->on_update(m_current_time);
            node_transform = bone->get_local_transform();
        }

        const glm::mat4 global_transformation = parent_transform * node_transform;

        const std::map<std::string, model_bone_info>& bone_info_map = m_animation_clip->get_model_bones_info();
        if (bone_info_map.contains(node_name))
        {
            const int index = bone_info_map.at(node_name).id;
            const glm::mat4 offset = bone_info_map.at(node_name).offset;
            m_final_bone_matrices[index] = global_transformation * offset;
        }

        for (int i = 0; i < animation_clip_node->children_count; i++)
            calculate_bone_transform(&animation_clip_node->children[i], global_transformation);
    }
}
