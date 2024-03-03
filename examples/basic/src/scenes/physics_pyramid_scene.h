#pragma once
#include "scene/scene.h"

class physics_pyramid_scene : public luly::scene::scene
{
public:
    physics_pyramid_scene();
    ~physics_pyramid_scene();

private:
    void initialize();

    void create_floor();
    void create_cube(glm::vec3 size, glm::vec3 pos);
    void create_cubes_pyramid();
};
