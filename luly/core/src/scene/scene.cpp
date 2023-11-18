#include "lypch.h"
#include "scene.h"

#include "actor/components/name_component.h"

namespace luly::scene
{
    scene::scene(const std::string& name)
    {
        m_name = name;

        LY_TRACE("Started creating scene...");
        LY_TRACE("  - Name: '{}'", m_name);

        m_actors_registry = std::make_unique<entt::registry>();

        LY_TRACE("Scene created successfully!");
    }

    scene::~scene()
    {
    }

    std::shared_ptr<scene_actor> scene::create_actor()
    {
        entt::entity actor_handle = m_actors_registry->create();
        const auto& actor = std::make_shared<scene_actor>(actor_handle, this);
        return actor;
    }

    std::shared_ptr<scene_actor> scene::create_actor(const std::string& name)
    {
        entt::entity actor_handle = m_actors_registry->create();
        const auto& actor = std::make_shared<scene_actor>(actor_handle, this);
        actor->add_component<name_component>(name);
        return actor;
    }
}
