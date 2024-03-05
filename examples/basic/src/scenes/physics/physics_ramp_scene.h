#pragma once

#include "scene/scene.h"

class physics_ramp_scene : public luly::scene::scene
{
public:
    physics_ramp_scene();
    ~physics_ramp_scene();

    void on_update(float delta_time) override;
    void on_img_gui() override;

private:
    void initialize();

    void create_ramp();
    void create_wall();
    void create_ball(const glm::vec3& position, float radius = 0.5f);

    float m_balls_elapsed_time;
    float m_balls_spawn_interval;
    bool m_spawn_balls;
    glm::vec3 m_balls_start_pos;
};
