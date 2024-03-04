#pragma once

#include "scene/scene.h"

#include <glm/glm.hpp>

class scene_utils
{
public:
    static void create_environment(luly::scene::scene* scene);
    static void create_floor(luly::scene::scene* scene);
    static std::shared_ptr<luly::scene::scene_actor> create_cube(luly::scene::scene* scene, glm::vec3 size, glm::vec3 pos);
};
