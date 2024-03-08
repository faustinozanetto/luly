#include "lypch.h"
#include "animation_bone.h"

#include <glm/gtx/quaternion.hpp>

namespace luly::renderer
{
    animation_bone::animation_bone(const std::string& name, int id, const aiNodeAnim* channel)
    {
        m_name = name;
        m_id = id;
        m_local_transform = glm::mat4(1.0f);

        m_num_positions = channel->mNumPositionKeys;
        for (int i = 0; i < m_num_positions; ++i)
        {
            const aiVector3D ai_position = channel->mPositionKeys[i].mValue;
            const float time_stamp = channel->mPositionKeys[i].mTime;
            bone_key_position data;
            data.position = glm::vec3(ai_position.x, ai_position.y, ai_position.z);
            data.time_stamp = time_stamp;
            m_positions.push_back(data);
        }

        m_num_rotations = channel->mNumRotationKeys;
        for (int i = 0; i < m_num_rotations; ++i)
        {
            const aiQuaternion ai_orientation = channel->mRotationKeys[i].mValue;
            const float time_stamp = channel->mRotationKeys[i].mTime;
            bone_key_rotation data;
            data.orientation = glm::quat(ai_orientation.w, ai_orientation.x, ai_orientation.y, ai_orientation.z);
            data.time_stamp = time_stamp;
            m_rotations.push_back(data);
        }

        m_num_scales = channel->mNumScalingKeys;
        for (int i = 0; i < m_num_scales; ++i)
        {
            const aiVector3D scale = channel->mScalingKeys[i].mValue;
            const float time_stamp = channel->mScalingKeys[i].mTime;
            bone_key_scale data;
            data.scale = glm::vec3(scale.x, scale.y, scale.z);
            data.time_stamp = time_stamp;
            m_scales.push_back(data);
        }
    }

    void animation_bone::on_update(float animation_time)
    {
        const glm::mat4 translation = interpolate_position(animation_time);
        const glm::mat4 rotation = interpolate_rotation(animation_time);
        const glm::mat4 scale = interpolate_scale(animation_time);
        m_local_transform = translation * rotation * scale;
    }

    int animation_bone::get_position_index(float animation_time) const
    {
        for (int index = 0; index < m_num_positions - 1; ++index)
        {
            if (animation_time < m_positions[index + 1].time_stamp)
                return index;
        }
        LY_ASSERT_MSG(false, "Invalid position index!")
        return -1;
    }

    int animation_bone::get_rotation_index(float animation_time) const
    {
        for (int index = 0; index < m_num_rotations - 1; ++index)
        {
            if (animation_time < m_rotations[index + 1].time_stamp)
                return index;
        }
        LY_ASSERT_MSG(false, "Invalid rotation index!")
        return -1;
    }

    int animation_bone::get_scale_index(float animation_time) const
    {
        for (int index = 0; index < m_num_scales - 1; ++index)
        {
            if (animation_time < m_scales[index + 1].time_stamp)
                return index;
        }
        LY_ASSERT_MSG(false, "Invalid scale index!")
        return -1;
    }

    float animation_bone::get_scale_factor(float last_time_stamp, float next_time_stamp, float animation_time)
    {
        const float mid_way_length = animation_time - last_time_stamp;
        const float frames_diff = next_time_stamp - last_time_stamp;
        const float scale_factor = mid_way_length / frames_diff;
        return scale_factor;
    }

    glm::mat4 animation_bone::interpolate_position(float animation_time)
    {
        if (1 == m_num_positions)
            return glm::translate(glm::mat4(1.0f), m_positions[0].position);

        const int p0_index = get_position_index(animation_time);
        const int p1_index = p0_index + 1;
        const float scale_factor = get_scale_factor(m_positions[p0_index].time_stamp,
                                                    m_positions[p1_index].time_stamp, animation_time);
        const glm::vec3 final_position = glm::mix(m_positions[p0_index].position, m_positions[p1_index].position
                                                  , scale_factor);
        return glm::translate(glm::mat4(1.0f), final_position);
    }

    glm::mat4 animation_bone::interpolate_rotation(float animation_time)
    {
        if (1 == m_num_rotations)
        {
            const glm::quat rotation = glm::normalize(m_rotations[0].orientation);
            return glm::toMat4(rotation);
        }

        const int p0_index = get_rotation_index(animation_time);
        const int p1_index = p0_index + 1;
        const float scale_factor = get_scale_factor(m_rotations[p0_index].time_stamp,
                                                    m_rotations[p1_index].time_stamp, animation_time);
        glm::quat final_rotation = glm::slerp(m_rotations[p0_index].orientation, m_rotations[p1_index].orientation
                                              , scale_factor);
        final_rotation = glm::normalize(final_rotation);
        return glm::toMat4(final_rotation);
    }

    glm::mat4 animation_bone::interpolate_scale(float animation_time)
    {
        if (1 == m_num_scales)
            return glm::scale(glm::mat4(1.0f), m_scales[0].scale);

        const int p0_index = get_scale_index(animation_time);
        const int p1_index = p0_index + 1;
        const float scale_factor = get_scale_factor(m_scales[p0_index].time_stamp,
                                                    m_scales[p1_index].time_stamp, animation_time);
        const glm::vec3 final_scale = glm::mix(m_scales[p0_index].scale, m_scales[p1_index].scale
                                               , scale_factor);
        return glm::scale(glm::mat4(1.0f), final_scale);
    }
}
