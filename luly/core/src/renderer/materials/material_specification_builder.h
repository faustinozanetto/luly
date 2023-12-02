#pragma once
#include "material.h"

namespace luly::renderer
{
    class material_specification_builder
    {
    public:
        material_specification_builder();

        /* Methods */
        material_specification_builder& with_albedo(const glm::vec3& value);
        material_specification_builder& with_emissive(const glm::vec3& value);
        material_specification_builder& with_roughness(float value);
        material_specification_builder& with_metallic(float value);
        material_specification_builder& with_ambient_occlusion(float value);
        material_specification_builder& with_tilling(float value);
        material_specification_builder& with_textures(const std::map<material_texture_type, material_texture>& value);
        material_specification_builder& with_emissive_strength(float value);
        /* Builder */
        material_specification build() const;

    private:
        material_specification m_material_specification;
    };
}
