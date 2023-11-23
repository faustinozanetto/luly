#include "lypch.h"
#include "material.h"

#include <format>

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
        initialize_texture_locations();
    }

    material::~material()
    {
    }

    void material::bind(const std::shared_ptr<shader>& shader)
    {
        shader->set_vec_float3("u_material.albedo", m_material_specification.albedo);
        shader->set_float("u_material.roughness", m_material_specification.roughness);
        shader->set_float("u_material.metallic", m_material_specification.metallic);
        shader->set_float("u_material.ambient_occlusion", m_material_specification.ambient_occlusion);
        shader->set_float("u_material.tilling", m_material_specification.tilling);

        for (auto& [type, texture] : m_material_specification.textures)
        {
            shader->set_int(m_material_texture_enabled_locations[type], texture.is_enabled ? 1 : 0);

            if (!texture.is_enabled || !texture.texture)
                continue;

            renderer::bind_texture(MATERIAL_TEXTURE_BIND_SLOTS.at(type), texture.texture->get_handle_id());
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

    void material::initialize_texture_locations()
    {
        for (material_texture_type type : MATERIAL_TEXTURE_TYPES)
        {
            std::string location = std::format("u_material.{}_map_enabled",
                                               material_utils::get_material_texture_type_to_string(type));
            m_material_texture_enabled_locations.insert(std::make_pair(type, location));
        }
    }
}
