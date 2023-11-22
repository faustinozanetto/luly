#include "lypch.h"
#include "material.h"

#include "renderer/renderer/renderer.h"

namespace luly::renderer
{
    material::material(const material_specification& material_specification)
    {
        m_material_specification = material_specification;
    }

    material::~material()
    {
    }

    void material::bind(const std::shared_ptr<shader>& shader)
    {
        shader->set_vec_float3("m_material_specification.albedo", m_material_specification.albedo);
        shader->set_float("u_material.roughness", m_material_specification.roughness);
        shader->set_float("u_material.metallic", m_material_specification.metallic);
        shader->set_float("u_material.ambient_occlusion", m_material_specification.ambient_occlusion);
        shader->set_float("u_material.tilling", m_material_specification.tilling);

        for (auto& [type, texture] : m_material_specification.textures)
        {
            shader->set_int(m_material_texture_enabled_locations[type], texture.is_enabled ? 1 : 0);
            if (!texture.is_enabled || !texture.texture)
                continue;
            const int bind_slot = m_material_texture_bindings[type];
            renderer::bind_texture(bind_slot, texture.texture->get_handle_id());
        }
    }

    void material::initialize_textures_map()
    {
        // Fill in the textures that were not provided in the constructor
        for (material_texture_type type : MATERIAL_TEXTURE_TYPES)
        {
            if (m_material_specification.textures.contains(type))
                continue;

            material_texture texture;
            texture.is_enabled = false;
            texture.type = type;
            texture.texture = nullptr;
            m_material_specification.textures[type] = texture;
        }
    }
}
