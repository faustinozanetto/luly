#pragma once
#include "animation_clip.h"

namespace luly::renderer
{
    class animation_controller
    {
    public:
        animation_controller(const std::shared_ptr<animation_clip>& animation_clip);
        ~animation_controller();

        /* Getters */
        const std::vector<glm::mat4>& get_final_bone_matrices() const { return m_final_bone_matrices; }
        const std::shared_ptr<animation_clip>& get_animation_clip() const { return m_animation_clip; }
        float get_speed_multiplier() const { return m_speed_multiplier; }
        float get_current_time() const { return m_current_time; }
        bool get_is_playing() const { return m_play_animation; }

        /* Setters */
        void set_speed_multiplier(float speed_multiplier) { m_speed_multiplier = speed_multiplier; }

        /* Methods */
        void on_update(float delta_time);
        void play();
        void pause();
        void resume();
        void stop();

        void calculate_bone_transform(const animation_clip_node* animation_clip_node, const glm::mat4& parent_transform);

    private:
        std::vector<glm::mat4> m_final_bone_matrices;
        std::shared_ptr<animation_clip> m_animation_clip;
        float m_current_time;
        float m_delta_time;
        float m_speed_multiplier;
        bool m_play_animation;
    };
}
