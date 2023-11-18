#pragma once

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

        /* Methods */
        std::shared_ptr<scene_actor> create_actor();
        std::shared_ptr<scene_actor> create_actor(const std::string& name);

    private:
        std::string m_name;
        std::unique_ptr<entt::registry> m_actors_registry;

        friend class scene_actor;
    };
}
