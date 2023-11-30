#include "lypch.h"
#include "material.h"

#include "material_utils.h"
#include "renderer/renderer/renderer.h"

namespace luly::renderer
{
    material_specification::material_specification(const glm::vec3& albedo,
                                                   float roughness,
                                                   float metallic,
                                                   float ambient_occlusion,
                                                   float tilling,
                                                   const std::map<material_texture_type, material_texture>& textures)
        : albedo(albedo), roughness(roughness), metallic(metallic),
          ambient_occlusion(ambient_occlusion), tilling(tilling), textures(textures)
    {
    }

    material::material(const material_specification& material_specification)
    {
        m_material_specification = material_specification;

        initialize_textures_map();
    }

    material::~material()
    {
    }

    void material::set_texture_enabled(material_texture_type texture_type, bool is_enabled)
    {
        m_material_specification.textures.at(texture_type).is_enabled = is_enabled;
    }

    void material::bind(const std::shared_ptr<shader>& shader)
    {
        shader->set_vec_float3("u_material.albedo", m_material_specification.albedo);
        shader->set_float("u_material.roughness", m_material_specification.roughness);
        shader->set_float("u_material.metallic", m_material_specification.metallic);
        shader->set_float("u_material.ambient_occlusion", m_material_specification.ambient_occlusion);
        shader->set_float("u_material.tilling", m_material_specification.tilling);

        for (auto& [texture_type, texture] : m_material_specification.textures)
        {
            shader->set_int(material_utils::get_material_texture_bind_location(texture_type),
                            texture.is_enabled ? 1 : 0);

            if (!texture.is_enabled || !texture.texture)
                continue;

            renderer::bind_texture(MATERIAL_TEXTURE_BIND_SLOTS.at(texture_type), texture.texture->get_handle_id());
        }
    }

    void material::bind_default(const std::shared_ptr<shader>& shader)
    {
        shader->set_vec_float3("u_material.albedo", {0.85f, 0.85f, 0.85f});
        shader->set_float("u_material.roughness", 1.0f);
        shader->set_float("u_material.metallic", 0.0f);
        shader->set_float("u_material.ambient_occlusion", 1.0f);
        shader->set_float("u_material.tilling", 1.0f);

        for (material_texture_type texture_type : MATERIAL_TEXTURE_TYPES)
        {
            shader->set_int(material_utils::get_material_texture_bind_location(texture_type), 0);
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
