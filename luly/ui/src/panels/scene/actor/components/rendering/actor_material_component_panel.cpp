#include "actor_material_component_panel.h"

#include "engine_ui.h"
#include "renderer/materials/material_utils.h"
#include "renderer/textures/texture_2d.h"
#include "renderer/textures/texture_factory.h"
#include "scene/actor/components/rendering/material_component.h"
#include "utils/file_utils.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_material_component_panel::actor_material_component_panel() : actor_component_panel(
        "Material Component")
    {
    }

    std::pair<bool, size_t> actor_material_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<scene::material_component>();
        auto component_hash = typeid(scene::material_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_material_component_panel::on_render_component_details()
    {
        auto& material_component = engine_ui::get_ui_data().selected_actor->get_component<
            scene::material_component>();

        if (material_component.get_material())
        {
            auto& material = material_component.get_material();
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            glm::vec3 albedo = material->get_albedo();
            if (ui_utils::draw_property("Albedo", albedo, true))
            {
                material->set_albedo(albedo);
            }

            float roughness = material->get_roughness();
            if (ui_utils::draw_property("Roughness", roughness, 0.0f, 1.0f, 0.01f))
            {
                material->set_roughness(roughness);
            }

            float metallic = material->get_metallic();
            if (ui_utils::draw_property("Metallic", metallic, 0.0f, 1.0f, 0.01f))
            {
                material->set_metallic(metallic);
            }

            float ambient_occlusion = material->get_ambient_occlusion();
            if (ui_utils::draw_property("Ambient Occlusion", ambient_occlusion, 0.0f, 1.0f, 0.01f))
            {
                material->set_ambient_occlusion(ambient_occlusion);
            }

            float tilling = material->get_tilling();
            if (ui_utils::draw_property("Tilling", tilling, 0.0f, 10.0f, 0.01f))
            {
                material->set_tilling(tilling);
            }

            ui_utils::draw_property("Textures");
            ImGui::Separator();
            const ImGuiTreeNodeFlags texture_flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
                ImGuiTreeNodeFlags_AllowItemOverlap;
            for (auto& [type, texture] : material->get_textures())
            {
                if (ImGui::TreeNodeEx(reinterpret_cast<void*>(type), texture_flags,
                                      renderer::material_utils::get_material_texture_type_to_string(type)))
                {
                    if (ui_utils::draw_property("Texture", texture.texture))
                    {
                        const std::string& texture_file_path = utils::file_utils::open_file_dialog(
                            "Texture File", {"*.png", "*.jpg"});
                        if (texture_file_path.empty())
                        {
                            LY_WARN("Failed to open texture file from dialog!");
                        }
                        else
                        {
                            texture.texture = renderer::texture_factory::create_texture_from_file(texture_file_path);
                        }
                    }

                    bool texture_enabled = texture.is_enabled;
                    if (ui_utils::draw_property("Enabled", texture_enabled))
                    {
                        material->set_texture_enabled(type, texture_enabled);
                    }
                    ImGui::TreePop();
                }
            }
        }
        else
        {
            ImGui::Text("No material currently loaded!");
        }
    }
}
