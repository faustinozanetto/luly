#pragma once

#include <PxPhysicsAPI.h>

namespace luly::physics
{
    class physics_material
    {
    public:
        physics_material(float static_friction = 0.5f, float dynamic_friction = 0.5f, float restitution = 0.6f);
        ~physics_material();

        /* Getters */
        float get_static_friction() const;
        float get_dynamic_friction() const;
        float get_restitution() const;
        physx::PxMaterial* get_physx_material() const { return m_material; }

        /* Setters */
        void set_static_friction(float static_friction) const;
        void set_dynamic_friction(float dynamic_friction) const;
        void set_restitution(float restitution) const;

    private:
        physx::PxMaterial* m_material;
    };
}
