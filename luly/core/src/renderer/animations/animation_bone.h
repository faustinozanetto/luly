#pragma once

#include <assimp/anim.h>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

namespace luly::renderer
{
    struct bone_key_position
    {
        glm::vec3 position;
        float time_stamp;
    };

    struct bone_key_rotation
    {
        glm::quat orientation;
        float time_stamp;
    };

    struct bone_key_scale
    {
        glm::vec3 scale;
        float time_stamp;
    };

    class animation_bone
    {
    public:
        animation_bone(const std::string& name, int id, const aiNodeAnim* channel);

        /* Getters */
        const glm::mat4& get_local_transform() const { return m_local_transform; }
        const std::string& get_name() const { return m_name; }
        int get_id() const { return m_id; }

        /* Methods */
        void on_update(float animation_time);
        int get_position_index(float animation_time) const;
        int get_rotation_index(float animation_time) const;
        int get_scale_index(float animation_time) const;

    private:
        float get_scale_factor(float last_time_stamp, float next_time_stamp, float animation_time);
        glm::mat4 interpolate_position(float animation_time);
        glm::mat4 interpolate_rotation(float animation_time);
        glm::mat4 interpolate_scale(float animation_time);
        
        std::vector<bone_key_position> m_positions;
        std::vector<bone_key_rotation> m_rotations;
        std::vector<bone_key_scale> m_scales;
        int m_num_positions;
        int m_num_rotations;
        int m_num_scales;

        glm::mat4 m_local_transform;
        std::string m_name;
        int m_id;
    };
}
