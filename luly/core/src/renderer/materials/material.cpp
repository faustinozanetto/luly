#include "lypch.h"
#include "material.h"

#include "material_utils.h"
#include "renderer/renderer/renderer.h"

namespace luly::renderer
{
    material_texture::material_texture()
    {
        this->texture = nullptr;
        this->type = material_texture_type::albedo;
        this->is_enabled = false;
    }

    material_texture::material_texture(const std::shared_ptr<texture_2d>& texture, material_texture_type type,
                                       bool is_enabled)
    {
        this->texture = texture;
        this->type = type;
        this->is_enabled = is_enabled;
    }

    material_specification::material_specification(const glm::vec4& albedo, const glm::vec4& emissive, float roughness,
                                                   float metallic, float ambient_occlusion, float tilling,
                                                   float emissive_strength,
                                                   const std::map<material_texture_type, material_texture>& textures)
        : albedo(albedo), emissive(emissive), roughness(roughness), metallic(metallic),
          ambient_occlusion(ambient_occlusion), tilling(tilling), emissive_strength(emissive_strength),
          textures(textures)
    {
    }

    material::material(const material_specification& material_specification)
    {
        m_material_specification = material_specification;

        initialize_textures_map();
        initialize_textures_channels_map();
    }

    material::~material()
    {
    }

    void material::set_texture_enabled(material_texture_type texture_type, bool is_enabled)
    {
        LY_PROFILE_FUNCTION;
        m_material_specification.textures.at(texture_type).is_enabled = is_enabled;
    }

    void material::set_texture_channel_mode(material_texture_type texture_type,
                                            material_texture_channel_mode texture_channel)
    {
        LY_PROFILE_FUNCTION;
        m_texture_type_channels.at(texture_type) = texture_channel;
    }

    void material::bind(const std::shared_ptr<shader>& shader)
    {
        LY_PROFILE_FUNCTION;
        shader->set_vec_float4("u_material.albedo", m_material_specification.albedo);
        shader->set_vec_float4("u_material.emissive", m_material_specification.emissive);
        shader->set_float("u_material.roughness", m_material_specification.roughness);
        shader->set_float("u_material.metallic", m_material_specification.metallic);
        shader->set_float("u_material.ambient_occlusion", m_material_specification.ambient_occlusion);
        shader->set_float("u_material.tilling", m_material_specification.tilling);
        shader->set_float("u_material.emissive_strength", m_material_specification.emissive_strength);

        for (auto& [texture_type, texture] : m_material_specification.textures)
        {
            shader->set_int(material_utils::get_material_texture_bind_location(texture_type),
                            texture.is_enabled ? 1 : 0);

            if (!texture.is_enabled || !texture.texture)
                continue;

            // Set material texture type channel packing mode.
            if (MATERIAL_TEXTURE_TYPE_SUPPORTS_CHANNEL_MODE.contains(texture_type))
            {
                std::string texture_channel_packing_location = std::format("u_material.{}_channel",
                                                                           material_utils::get_material_texture_type_to_string(
                                                                               texture_type));
                shader->set_int(texture_channel_packing_location,
                                static_cast<int>(m_texture_type_channels.at(texture_type)));
            }

            renderer::bind_texture(MATERIAL_TEXTURE_BIND_SLOTS.at(texture_type), texture.texture->get_handle_id());
        }
    }

    void material::bind_default(const std::shared_ptr<shader>& shader)
    {
        LY_PROFILE_FUNCTION;
        shader->set_vec_float4("u_material.albedo", {0.85f, 0.85f, 0.85f, 1.0f});
        shader->set_vec_float4("u_material.emissive", {0.0f, 0.0f, 0.0f, 1.0f});
        shader->set_float("u_material.roughness", 0.99f);
        shader->set_float("u_material.metallic", 0.01f);
        shader->set_float("u_material.ambient_occlusion", 1.0f);
        shader->set_float("u_material.tilling", 1.0f);
        shader->set_float("u_material.emissive_strength", 0.0f);

        for (const material_texture_type texture_type : MATERIAL_TEXTURE_TYPES)
        {
            shader->set_int(material_utils::get_material_texture_bind_location(texture_type), 0);
        }
    }

    void material::initialize_textures_map()
    {
        LY_PROFILE_FUNCTION;
        // Fill in the textures that were not provided in the constructor
        for (material_texture_type type : MATERIAL_TEXTURE_TYPES)
        {
            if (m_material_specification.textures.contains(type))
                continue;

            const material_texture texture = {nullptr, type, false};
            m_material_specification.textures[type] = texture;
        }
    }

    void material::initialize_textures_channels_map()
    {
        LY_PROFILE_FUNCTION;
        for (material_texture_type type : MATERIAL_TEXTURE_TYPES)
        {
            if (MATERIAL_TEXTURE_TYPE_SUPPORTS_CHANNEL_MODE.contains(type))
                m_texture_type_channels.insert({type, material_texture_channel_mode::red});
        }
    }
}
