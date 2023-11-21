#pragma once

#include "scene/scene.h"

#include "core_api.h"

#include <logging/log.h>
#include <utils/assert.h>
#include <entt/entt.hpp>

namespace luly::scene
{
    class LULY_CORE_API scene_actor
    {
    public:
        scene_actor(entt::entity handle, scene* scene);
        scene_actor(const scene_actor& other) = default;

        /* Getters */
        entt::entity get_handle() const { return m_handle; }

        /* Methods */
        template <typename T, typename... Args>
        T& add_component(Args&&... args)
        {
            LY_ASSERT_MSG(!has_component<T>(), "Scene actor already has that component!");
            auto& component = m_scene->get_registry()->emplace<T>(m_handle, std::forward<Args>(args)...);
            component.set_actor(this);
            component.initialize();
            return component;
        }

        template <typename T>
        T& get_component()
        {
            LY_ASSERT_MSG(has_component<T>(), "Scene actor does not have that component!");
            return m_scene->get_registry()->get<T>(m_handle);
        }

        template <typename T>
        bool has_component() const
        {
            return m_scene->get_registry()->any_of<T>(m_handle);
        }

        template <typename T>
        void remove_component() const
        {
            LY_ASSERT_MSG(has_component<T>(), "Scene actor does not have that component!");
            m_scene->get_registry()->remove<T>(m_handle);
        }

        /* Operators */
        operator bool() const { return m_handle != entt::null; }

        bool operator==(const scene_actor& other) const
        {
            return m_handle == other.m_handle && m_scene == other.m_scene;
        }

        bool operator!=(const scene_actor& other) const
        {
            return !(*this == other);
        }

    private:
        entt::entity m_handle;
        scene* m_scene;
    };
}
