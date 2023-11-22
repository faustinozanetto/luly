#pragma once

#include "scene/actor/scene_actor.h"
#include "utils/uuid.h"


namespace luly::scene
{
    class base_component
    {
    public:
        virtual ~base_component() = default;

        /* Getters */
        const utils::uuid& get_uuid() const { return m_uuid; }
        scene_actor* get_actor() const { return m_actor; }

        /* Setters */
        void set_actor(scene_actor* actor) { m_actor = actor; }

        virtual void initialize() = 0;

    protected:
        utils::uuid m_uuid;
        scene_actor* m_actor = nullptr;
    };
}
