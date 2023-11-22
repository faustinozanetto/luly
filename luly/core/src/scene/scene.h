#pragma once

#include "renderer/camera/camera_manager.h"
#include <entt/entt.hpp>

namespace luly::scene
{
    class scene_actor;

    class scene
    {
    public:
        scene(const std::string& name);
        ~scene();

        /* Getters*/
        const std::string& get_name() const { return m_name; }
        std::unique_ptr<entt::registry>& get_registry() { return m_actors_registry; }
        const std::shared_ptr<scene_actor>& get_actor(entt::entity entity);
        const std::shared_ptr<renderer::camera_manager>& get_camera_manager() const { return m_camera_manager; }

        /* Methods */
        std::shared_ptr<scene_actor> create_actor(const std::string& name);

    private:
        std::string m_name;
        std::unique_ptr<entt::registry> m_actors_registry;
        std::unordered_map<entt::entity, std::shared_ptr<scene_actor>> m_actors_map;
        std::shared_ptr<renderer::camera_manager> m_camera_manager;

        friend class scene_actor;
    };
}
