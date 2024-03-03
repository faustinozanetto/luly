#include "lifetime_component.h"

#include "scene/scene_manager.h"

lifetime_component::lifetime_component(float lifetime)
{
    m_lifetime = lifetime;
}

void lifetime_component::initialize()
{
}

void lifetime_component::on_update(float delta_time)
{
    m_lifetime -= delta_time;
    if (m_lifetime <= 0)
    {
        luly::scene::scene_manager::get().get_current_scene()->delete_actor(m_actor->get_handle());
        LY_TRACE("LIFETIME COMPLETED");
    }
}
