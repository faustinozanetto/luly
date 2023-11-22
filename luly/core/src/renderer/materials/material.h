#pragma once

#include "renderer/textures/texture.h"

#include <glm/glm.hpp>

#include "renderer/shaders/shader.h"

namespace luly::renderer
{
    enum class material_texture_type
    {
        albedo,
        normal,
        roughness,
        metallic,
        ambient_occlusion,
        emissive,
        opacity
    };

    struct material_texture
    {
        std::shared_ptr<texture> texture;
        material_texture_type type;
        bool is_enabled;
    };

    struct material_specification
    {
        glm::vec3 albedo;
        float roughness;
        float metallic;
        float ambient_occlusion;
        float tilling;
        std::map<material_texture_type, material_texture> textures;

        material_specification(const glm::vec3& albedo = glm::vec3(0.0f),
                               float roughness = 1.0f,
                               float metallic = 0.0f,
                               float ambient_occlusion = 1.0f,
                               float tilling = 1.0f,
                               const std::map<material_texture_type, material_texture>& textures = {});
    };

    constexpr material_texture_type MATERIAL_TEXTURE_TYPES[] = {
        material_texture_type::albedo,
        material_texture_type::normal,
        material_texture_type::roughness,
        material_texture_type::metallic,
        material_texture_type::ambient_occlusion,
        material_texture_type::emissive,
        material_texture_type::opacity
    };

    class material
    {
    public:
        material(const material_specification& material_specification);
        ~material();

        /* Methods */
        void bind(const std::shared_ptr<shader>& shader);

    private:
        void initialize_textures_map();

        material_specification m_material_specification;
    };
}
