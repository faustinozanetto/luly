#pragma once

#include "scene/scene.h"

namespace luly::scene
{
    class scene_manager
    {
    protected:
        scene_manager();

    public:
        ~scene_manager();

        /* Getters */
        static scene_manager& get();
        const std::shared_ptr<scene>& get_current_scene() const { return m_current_scene; }
        const std::set<std::shared_ptr<scene>>& get_scenes() const { return m_scenes_pool; }

        /* Methods*/
        static void initialize();
        void add_scene(const std::shared_ptr<scene>& scene);
        void switch_scene(const std::shared_ptr<scene>& scene, bool update_title = true);

        void on_update(float delta_time) const;
        void on_resize(const glm::ivec2& dimensions) const;

    private:
        std::set<std::shared_ptr<scene>> m_scenes_pool;
        std::shared_ptr<scene> m_current_scene;

        /* Singleton Instance */
        static scene_manager* s_instance;
    };
}
