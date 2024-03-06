#include "physics_tower_scene.h"

#include "physics/physics_utils.h"
#include "physics/actors/physics_dynamic_actor.h"
#include "scene/actor/components/transform_component.h"
#include "scenes/scene_utils.h"

physics_tower_scene::physics_tower_scene() : scene("Physics Tower Scene")
{
    initialize();
}

physics_tower_scene::~physics_tower_scene()
{
}

void physics_tower_scene::initialize()
{
    scene_utils::create_environment(this);
    scene_utils::create_floor(this);
    create_cubes_tower();
}

void physics_tower_scene::create_cubes_tower()
{
    int size = 5;

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            for (int z = 0; z < size; z++)
            {
                glm::vec3 cube_pos = {
                    (float)x, (float)y + 1.0f, (float)z,
                };
                scene_utils::create_cube(this, glm::vec3(1.0f), cube_pos);
            }
        }
    }
}
