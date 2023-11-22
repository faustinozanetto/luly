#include "lypch.h"
#include "material_specification_builder.h"

namespace luly::renderer
{
    material_specification_builder::material_specification_builder()
    {
        m_material_specification = {};
    }

    material_specification_builder& material_specification_builder::with_albedo(const glm::vec3& value)
    {
        m_material_specification.albedo = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_roughness(float value)
    {
        m_material_specification.roughness = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_metallic(float value)
    {
        m_material_specification.metallic = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_ambient_occlusion(float value)
    {
        m_material_specification.ambient_occlusion = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_tilling(float value)
    {
        m_material_specification.tilling = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_textures(
        const std::map<material_texture_type, material_texture>& value)
    {
        m_material_specification.textures = value;
        return *this;
    }

    material_specification material_specification_builder::build() const
    {
        return m_material_specification;
    }
}
