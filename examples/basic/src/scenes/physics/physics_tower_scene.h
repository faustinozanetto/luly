#pragma once

#include "scene/scene.h"

class physics_tower_scene : public luly::scene::scene
{
public:
    physics_tower_scene();
    ~physics_tower_scene();

private:
    void initialize();

    void create_cubes_tower();
};
