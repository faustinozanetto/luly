#include "lypch.h"
#include "material_specification_builder.h"

namespace luly::renderer
{
    material_specification_builder::material_specification_builder()
    {
        m_material_specification = {};
    }

    material_specification_builder& material_specification_builder::with_albedo(const glm::vec4& value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.albedo = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_emissive(const glm::vec4& value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.emissive = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_roughness(float value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.roughness = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_metallic(float value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.metallic = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_ambient_occlusion(float value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.ambient_occlusion = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_tilling(float value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.tilling = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_textures(
        const std::map<material_texture_type, material_texture>& value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.textures = value;
        return *this;
    }

    material_specification_builder& material_specification_builder::with_emissive_strength(float value)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.emissive_strength = value;
        return *this;
    }

    material_specification material_specification_builder::build() const
    {
        LY_PROFILE_FUNCTION;
        return m_material_specification;
    }
}
