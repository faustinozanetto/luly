﻿#pragma once

#include "scene/scene.h"

#include <glm/glm.hpp>

class scene_utils
{
public:
    static std::shared_ptr<luly::scene::scene_actor> create_point_light(luly::scene::scene* scene, const glm::vec3& position, const glm::vec3& color);
    static void create_environment(luly::scene::scene* scene);
    static void create_floor(luly::scene::scene* scene);
    static std::shared_ptr<luly::scene::scene_actor> create_cube(luly::scene::scene* scene, glm::vec3 size, glm::vec3 pos);
    static std::shared_ptr<luly::scene::scene_actor> create_static_cube(luly::scene::scene* scene, glm::vec3 size, glm::vec3 pos);
};
