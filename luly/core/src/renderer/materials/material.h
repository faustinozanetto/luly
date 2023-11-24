#pragma once

#include "renderer/textures/texture.h"
#include "renderer/shaders/shader.h"

#include <glm/glm.hpp>
#include <map>

namespace luly::renderer
{
    enum class material_texture_type
    {
        albedo,
        normal,
        roughness,
        metallic,
        ambient_occlusion,
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

        material_specification(const glm::vec3& albedo = glm::vec3(1.0f),
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
        material_texture_type::opacity,
    };

    const std::map<material_texture_type, int> MATERIAL_TEXTURE_BIND_SLOTS = {
        {material_texture_type::albedo, 0},
        {material_texture_type::normal, 1},
        {material_texture_type::roughness, 2},
        {material_texture_type::metallic, 3},
        {material_texture_type::ambient_occlusion, 4},
        {material_texture_type::opacity, 5},
    };

    class material
    {
    public:
        material(const material_specification& material_specification);
        ~material();

        /* Getters */
        const glm::vec3& get_albedo() const { return m_material_specification.albedo; }
        float get_ambient_occlusion() const { return m_material_specification.ambient_occlusion; }
        float get_tilling() const { return m_material_specification.tilling; }
        float get_roughness() const { return m_material_specification.roughness; }
        float get_metallic() const { return m_material_specification.metallic; }

        const std::map<material_texture_type, material_texture>& get_textures() const
        {
            return m_material_specification.textures;
        }

        /* Setters */
        void set_texture_enabled(material_texture_type texture_type, bool is_enabled);
        void set_albedo(const glm::vec3& albedo) { m_material_specification.albedo = albedo; }

        void set_ambient_occlusion(float ambient_occlusion)
        {
            m_material_specification.ambient_occlusion = ambient_occlusion;
        }

        void set_tilling(float tilling) { m_material_specification.tilling = tilling; }
        void set_roughness(float roughness) { m_material_specification.roughness = roughness; }
        void set_metallic(float metallic) { m_material_specification.metallic = metallic; }

        /* Methods */
        void bind(const std::shared_ptr<shader>& shader);
        static void bind_default(const std::shared_ptr<shader>& shader);

    private:
        void initialize_textures_map();

        material_specification m_material_specification;
    };
}
