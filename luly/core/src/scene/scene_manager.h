#pragma once

#include "scene/scene.h"
#include "core_api.h"

namespace luly::scene
{
    class LULY_CORE_API scene_manager
    {
    public:
        scene_manager();
        ~scene_manager();

        /* Getters */
        const std::shared_ptr<scene>& get_current_scene() const { return m_current_scene; }

        /* Methods*/
        void add_scene(const std::shared_ptr<scene>& scene);
        void switch_scene(const std::shared_ptr<scene>& scene);

    private:
        std::set<std::shared_ptr<scene>> m_scenes_pool;
        std::shared_ptr<scene> m_current_scene;
    };
}
