#include "lypch.h"
#include "scene_serializer.h"

#include "renderer/materials/material_utils.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/identifier_component.h"
#include "scene/actor/components/name_component.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/physics/physics_material_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_box_collision_shape_component.h"
#include "scene/actor/components/physics/collision_shapes/physics_sphere_collision_shape_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/rendering/skybox_component.h"

namespace YAML
{
    template <>
    struct convert<glm::vec2>
    {
        static Node encode(const glm::vec2& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec2& rhs)
        {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

    template <>
    struct convert<glm::quat>
    {
        static Node encode(const glm::quat& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::quat& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.w = node[0].as<float>();
            rhs.x = node[1].as<float>();
            rhs.y = node[2].as<float>();
            rhs.z = node[3].as<float>();
            return true;
        }
    };


    template <>
    struct convert<luly::utils::uuid>
    {
        static Node encode(const luly::utils::uuid& rhs)
        {
            Node node;
            node.push_back(rhs);
            return node;
        }

        static bool decode(const Node& node, luly::utils::uuid& rhs)
        {
            rhs = node.as<luly::utils::uuid>();
            return true;
        }
    };
}

namespace luly::scene
{
    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4& vec)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << vec.x << vec.y << vec.z << vec.w << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator <<(YAML::Emitter& out, const glm::quat& quat)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << quat.w << quat.x << quat.y << quat.z << YAML::EndSeq;
        return out;
    }

    scene_serializer::scene_serializer(const std::shared_ptr<scene>& scene)
    {
        m_scene = scene;
    }

    void scene_serializer::serialize(const std::string& file_path)
    {
        LY_TRACE("Started serializing scene with name: '{}' to file: '{}'...", m_scene->get_name(), file_path);

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "scene";
        out << YAML::Value << m_scene->get_name();

        out << YAML::Key << "actors";
        out << YAML::Value << YAML::BeginSeq;
        m_scene->get_registry()->each([&](auto actor_id)
        {
            const std::shared_ptr<scene_actor>& scene_actor = m_scene->get_actor(actor_id);
            if (scene_actor) serialize_actor(out, scene_actor);
        });
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream file(file_path);
        file << out.c_str();

        LY_TRACE("Scene serialized successfully!");
    }

    bool scene_serializer::deserialize(const std::string& file_path)
    {
        LY_TRACE("Started deserializing scene from file: '{}'...", file_path);

        std::ifstream stream(file_path);
        std::stringstream string_stream;
        string_stream << stream.rdbuf();

        YAML::Node data = YAML::Load(string_stream.str());
        if (!data["scene"]) return false;

        std::string scene_name = data["scene"].as<std::string>();
        LY_TRACE("Found scene name: '{}'.", scene_name);

        YAML::Node actors = data["actors"];
        if (actors)
        {
            for (auto actor : actors)
            {
                // 1. Deserialize uuid and name component.
                std::string name;
                utils::uuid identifier = actor["actor"].as<utils::uuid>();
                YAML::Node name_component = actor["name_component"];
                if (name_component)
                {
                    name = name_component["name"].as<std::string>();
                }

                // 2. Create scene actor.
                std::shared_ptr<scene_actor> scene_actor = m_scene->create_actor_with_uuid(name, identifier);

                // 3. Deserialize all the other components
                if (YAML::Node transform_component = actor["transform_component"])
                {
                    class transform_component& tc = scene_actor->get_component<class transform_component>();
                    tc.get_transform()->set_location(transform_component["location"].as<glm::vec3>());
                    tc.get_transform()->set_rotation(transform_component["rotation"].as<glm::quat>());
                    tc.get_transform()->set_scale(transform_component["scale"].as<glm::vec3>());
                }
            }
        }
    }

    void scene_serializer::serialize_actor(YAML::Emitter& out, const std::shared_ptr<scene_actor>& scene_actor)
    {
        out << YAML::BeginMap;
        const utils::uuid& actor_identifier = scene_actor->get_component<identifier_component>().get_identifier();
        out << YAML::Key << "actor" << YAML::Value << actor_identifier;

        if (scene_actor->has_component<name_component>())
        {
            out << YAML::Key << "name_component";
            out << YAML::BeginMap;

            const name_component& name_component = scene_actor->get_component<class name_component>();
            out << YAML::Key << "name" << YAML::Value << name_component.get_name();

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<transform_component>())
        {
            out << YAML::Key << "transform_component";
            out << YAML::BeginMap;

            const transform_component& transform_component = scene_actor->get_component<class transform_component>();
            const std::shared_ptr<math::transform>& transform = transform_component.get_transform();
            out << YAML::Key << "location" << YAML::Value << transform->get_location();
            out << YAML::Key << "rotation" << YAML::Value << transform->get_rotation();
            out << YAML::Key << "scale" << YAML::Value << transform->get_scale();

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<model_renderer_component>())
        {
            out << YAML::Key << "model_renderer_component";
            out << YAML::BeginMap;

            const model_renderer_component& model_renderer_component = scene_actor->get_component<class
                model_renderer_component>();
            out << YAML::Key << "casts_shadows" << YAML::Value << model_renderer_component.get_casts_shadows();
            out << YAML::Key << "model" << YAML::Value;
            out << YAML::BeginMap;
            // TODO: model storing logic.
            out << YAML::EndMap;

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<material_component>())
        {
            out << YAML::Key << "material_component";
            out << YAML::BeginMap;

            const material_component& material_component = scene_actor->get_component<class
                material_component>();
            const std::shared_ptr<renderer::material>& material = material_component.get_material();

            out << YAML::Key << "albedo" << YAML::Value << material->get_albedo();
            out << YAML::Key << "emissive" << YAML::Value << material->get_emissive();
            out << YAML::Key << "roughness" << YAML::Value << material->get_roughness();
            out << YAML::Key << "metallic" << YAML::Value << material->get_metallic();
            out << YAML::Key << "ambient_occlusion" << YAML::Value << material->get_ambient_occlusion();
            out << YAML::Key << "tilling" << YAML::Value << material->get_tilling();
            out << YAML::Key << "emissive_strength" << YAML::Value << material->get_emissive_strength();

            {
                out << YAML::Key << "textures" << YAML::Value;
                out << YAML::BeginMap;
                for (const auto& material_texture : material->get_textures())
                {
                    out << YAML::Key << renderer::material_utils::get_material_texture_type_to_string(
                        material_texture.first) << YAML::Value;
                    out << YAML::BeginMap;
                    out << YAML::Key << "is_enabled" << YAML::Value << material_texture.second.is_enabled;
                    out << YAML::EndMap;
                }
                out << YAML::EndMap;
            }

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<skybox_component>())
        {
            out << YAML::Key << "skybox_component";
            out << YAML::BeginMap;

            const skybox_component& skybox_component = scene_actor->get_component<class
                skybox_component>();

            out << YAML::Key << "tint" << YAML::Value << skybox_component.get_tint();
            out << YAML::Key << "intensity" << YAML::Value << skybox_component.get_intensity();
            out << YAML::Key << "lod_level" << YAML::Value << skybox_component.get_lod_level();
            // TODO: environment texture store logic.

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<directional_light_component>())
        {
            out << YAML::Key << "directional_light_component";
            out << YAML::BeginMap;

            const directional_light_component& directional_light_component = scene_actor->get_component<class
                directional_light_component>();
            const std::shared_ptr<renderer::directional_light>& directional_light = directional_light_component.
                get_directional_light();

            out << YAML::Key << "directional_light" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "color" << YAML::Value << directional_light->get_color();
            out << YAML::Key << "intensity" << YAML::Value << directional_light->get_intensity();
            out << YAML::Key << "direction_angles" << YAML::Value << directional_light->get_direction_angles();
            out << YAML::Key << "cascade_split_lambda" << YAML::Value << directional_light->get_cascade_split_lambda();
            out << YAML::Key << "shadow_map_dimensions" << YAML::Value << directional_light->
                get_shadow_map_dimensions();
            out << YAML::EndMap;
            out << YAML::Key << "enable_shadows" << YAML::Value << directional_light_component.get_enable_shadows();

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<point_light_component>())
        {
            out << YAML::Key << "point_light_component";
            out << YAML::BeginMap;

            const point_light_component& point_light_component = scene_actor->get_component<class
                point_light_component>();
            const std::shared_ptr<renderer::point_light>& point_light = point_light_component.
                get_point_light();

            out << YAML::Key << "point_light" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "color" << YAML::Value << point_light->get_color();
            out << YAML::Key << "intensity" << YAML::Value << point_light->get_intensity();
            out << YAML::Key << "position" << YAML::Value << point_light->get_position();
            out << YAML::Key << "constant_factor" << YAML::Value << point_light->get_constant_factor();
            out << YAML::Key << "linear_factor" << YAML::Value << point_light->get_linear_factor();
            out << YAML::Key << "quadratic_factor" << YAML::Value << point_light->get_quadratic_factor();
            out << YAML::Key << "shadow_map_dimensions" << YAML::Value << point_light->
                get_shadow_map_dimensions();
            out << YAML::Key << "shadow_map_near_plane" << YAML::Value << point_light->
                get_shadow_map_near_plane();
            out << YAML::Key << "shadow_map_far_plane" << YAML::Value << point_light->
                get_shadow_map_far_plane();
            out << YAML::EndMap;
            out << YAML::Key << "enable_shadows" << YAML::Value << point_light_component.get_enable_shadows();

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<physics_material_component>())
        {
            out << YAML::Key << "physics_material_component";
            out << YAML::BeginMap;

            const physics_material_component& physics_material_component = scene_actor->get_component<class
                physics_material_component>();
            const std::shared_ptr<physics::physics_material>& physics_material = physics_material_component.
                get_physics_material();

            out << YAML::Key << "physics_material" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "static_friction" << YAML::Value << physics_material->get_static_friction();
            out << YAML::Key << "dynamic_friction" << YAML::Value << physics_material->get_dynamic_friction();
            out << YAML::Key << "restitution" << YAML::Value << physics_material->get_restitution();
            out << YAML::EndMap;

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<physics_sphere_collision_shape_component>())
        {
            out << YAML::Key << "physics_sphere_collision_shape_component";
            out << YAML::BeginMap;

            const physics_sphere_collision_shape_component& physics_sphere_collision_shape_component = scene_actor->
                get_component<class
                    physics_sphere_collision_shape_component>();
            const std::shared_ptr<physics::physics_sphere_collision>& physics_sphere_collision =
                physics_sphere_collision_shape_component.
                get_sphere_collision();

            out << YAML::Key << "physics_sphere_collision" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "radius" << YAML::Value << physics_sphere_collision->get_radius();
            out << YAML::EndMap;

            out << YAML::EndMap;
        }

        if (scene_actor->has_component<physics_box_collision_shape_component>())
        {
            out << YAML::Key << "physics_box_collision_shape_component";
            out << YAML::BeginMap;

            const physics_box_collision_shape_component& physics_box_collision_shape_component = scene_actor->
                get_component<class
                    physics_box_collision_shape_component>();
            const std::shared_ptr<physics::physics_box_collision>& physics_box_collision =
                physics_box_collision_shape_component.
                get_box_collision();

            out << YAML::Key << "physics_box_collision" << YAML::Value;
            out << YAML::BeginMap;
            out << YAML::Key << "half_extents" << YAML::Value << physics_box_collision->get_half_extents();
            out << YAML::EndMap;

            out << YAML::EndMap;
        }

        out << YAML::EndMap;
    }
}
