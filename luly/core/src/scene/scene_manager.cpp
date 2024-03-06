#include "lypch.h"
#include "scene_manager.h"

#include "application/application.h"
#include "renderer/scene/scene_renderer.h"

namespace luly::scene
{
    scene_manager* scene_manager::s_instance = nullptr;

    scene_manager::scene_manager()
    {
        LY_INFO("Started creating scene manager...");
        s_instance = this;
        m_scenes_pool = std::set<std::shared_ptr<scene>>();
        LY_INFO("Scene manager created successfully!");
    }

    scene_manager::~scene_manager()
    {
    }

    scene_manager& scene_manager::get()
    {
        LY_ASSERT_MSG(s_instance, "Tried to get scene_manager instance when not initialized!");
        return *s_instance;
    }

    void scene_manager::add_scene(const std::shared_ptr<scene>& scene)
    {
        m_scenes_pool.insert(scene);
    }

    void scene_manager::switch_scene(const std::shared_ptr<scene>& scene, bool update_title)
    {
        m_current_scene = scene;

        if (update_title)
        {
            const std::shared_ptr<renderer::window>& window = core::application::get().get_window();
            const std::string new_title = core::application::get().get_initial_title() + " | Scene: " + scene->
                get_name();
            window->set_title(new_title);
        }

        renderer::scene_renderer::set_outputs();
    }

    void scene_manager::on_update(float delta_time)
    {
        if (m_current_scene)
        {
            m_current_scene->handle_delete_entities();
            m_current_scene->on_update(delta_time);
        }
    }

    void scene_manager::on_resize(const glm::ivec2& dimensions) const
    {
        if (m_current_scene)
        {
            m_current_scene->get_camera_manager()->get_perspective_camera()->update_projection_matrix(dimensions);
        }
    }

    void scene_manager::initialize()
    {
        s_instance = new scene_manager();
    }
}
