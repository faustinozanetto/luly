#pragma once

#include "scene/scene.h"

class physics_joints_scene : public luly::scene::scene
{
public:
    physics_joints_scene();
    ~physics_joints_scene();

private:
    void initialize();

    void create_fixed_joint_example();
    void create_chain();
};
