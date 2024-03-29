﻿#pragma once

#include "renderer/camera/camera_manager.h"
#include "utils/uuid.h"

#include <entt/entt.hpp>
#include <unordered_set>
#include <PxScene.h>

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
        physx::PxScene* get_physx_scene() const { return m_physx_scene; }
        const glm::vec3& get_gravity() const { return m_gravity; }

        /* Setters */
        void set_gravity(const glm::vec3& gravity);

        /* Methods */
        std::shared_ptr<scene_actor> create_actor(const std::string& name);
        std::shared_ptr<scene_actor> create_actor_with_uuid(const std::string& name, const utils::uuid& uuid);
        void delete_actor(entt::entity handle);
        void handle_delete_entities() const;

        /* Virtuals */
        virtual void on_update(float delta_time);

        virtual void on_img_gui()
        {
        }

        /** Common Actors Getters */
        std::vector<directional_light_component> get_directional_light() const;
        std::vector<point_light_component> get_point_lights() const;
        class skybox_component* get_skybox() const;

    private:
        void initialize_physx_scene();
        void update_lights() const;
        void update_animation_controllers(float delta_time) const;

        // Common
        std::string m_name;
        std::shared_ptr<renderer::camera_manager> m_camera_manager;

        // Actors
        std::unique_ptr<entt::registry> m_actors_registry;
        std::unordered_map<entt::entity, std::shared_ptr<scene_actor>> m_actors_map;
        std::unordered_set<entt::entity> m_pending_delete_actors;

        // Physics
        physx::PxScene* m_physx_scene;
        glm::vec3 m_gravity;

        friend class scene_actor;
        friend class scene_serializer;
    };
}
