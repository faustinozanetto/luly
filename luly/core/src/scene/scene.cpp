#include "lypch.h"
#include "scene.h"

#include "actor/components/name_component.h"
#include "actor/components/transform_component.h"

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

    const std::shared_ptr<scene_actor>& scene::get_actor(entt::entity entity)
    {
        const auto iterator = m_actors_map.find(entity);
        if (iterator == m_actors_map.end())
            LY_ASSERT_MSG(m_actors_map.contains(entity), "Actor does not belong to the registry!")

        return iterator->second;
    }

    std::shared_ptr<scene_actor> scene::create_actor(const std::string& name)
    {
        entt::entity actor_handle = m_actors_registry->create();
        const auto& actor = std::make_shared<scene_actor>(actor_handle, this);
        actor->add_component<name_component>(name);
        actor->add_component<transform_component>();

        m_actors_map.insert({actor_handle, actor});

        return actor;
    }
}
