#include "lypch.h"
#include "scene_actor.h"

namespace luly::scene
{
    scene_actor::scene_actor(entt::entity handle, scene* scene)
    {
        m_handle = handle;
        m_scene = scene;
    }
}
