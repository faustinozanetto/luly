﻿#pragma once

#include "renderer/camera/camera_manager.h"
#include "utils/uuid.h"

#include <entt/entt.hpp>
#include <unordered_set>

namespace luly::scene
{
    class base_component;
    class scene_actor;
    class point_light_component;
    class directional_light_component;

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
        void delete_actor(entt::entity handle);
        void on_update(float delta_time);
        void handle_delete_entities();

        /** Common Actors Getters */
        std::vector<directional_light_component> get_directional_light() const;
        std::vector<point_light_component> get_point_lights() const;
        const std::shared_ptr<scene_actor>& get_skybox_actor() const;

    private:
        void update_lights();

        std::string m_name;
        std::shared_ptr<renderer::camera_manager> m_camera_manager;

        std::unique_ptr<entt::registry> m_actors_registry;
        std::unordered_map<entt::entity, std::shared_ptr<scene_actor>> m_actors_map;
        std::unordered_set<entt::entity> m_pending_delete_entities;

        friend class scene_actor;
    };
}
