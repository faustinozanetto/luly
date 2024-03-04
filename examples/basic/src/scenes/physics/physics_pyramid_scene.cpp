#include "physics_pyramid_scene.h"

#include "physics/physics_utils.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "scene/actor/components/transform_component.h"
#include "scenes/scene_utils.h"

physics_pyramid_scene::physics_pyramid_scene() : scene("Physics Pyramid Scene")
{
    initialize();
}

physics_pyramid_scene::~physics_pyramid_scene()
{
}

void physics_pyramid_scene::initialize()
{
    scene_utils::create_environment(this);
    scene_utils::create_floor(this);
    create_cubes_pyramid();
}

void physics_pyramid_scene::create_cubes_pyramid()
{
    int levels = 8;
    glm::vec3 start_pos = {0, 4, 0};

    for (int i = 0; i < levels; i++)
    {
        int cubes_count = levels - i;
        float half_cube_width = 0.5f;
        for (int j = 0; j < cubes_count; j++)
        {
            const glm::vec3 cube_pos = {
                start_pos.x + j - (cubes_count - 1) * 0.5f + half_cube_width, start_pos.y + i, 0
            };
            scene_utils::create_cube(this, glm::vec3(1.0f), cube_pos);
        }
    }
}
