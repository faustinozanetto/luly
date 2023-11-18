#include "lypch.h"
#include "scene_manager.h"

namespace luly::scene
{
    scene_manager::scene_manager()
    {
        m_scenes_pool = std::set<std::shared_ptr<scene>>();
    }

    scene_manager::~scene_manager()
    {
    }

    void scene_manager::add_scene(const std::shared_ptr<scene>& scene)
    {
        m_scenes_pool.insert(scene);
    }
}
