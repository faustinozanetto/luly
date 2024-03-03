#include "actor_material_component_panel.h"

#include "engine_ui.h"
#include "imgui_internal.h"
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
        bool has_component = engine_ui::get().get_selected_actor()->has_component<scene::material_component>();
        auto component_hash = typeid(scene::material_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_material_component_panel::on_render_component_details()
    {
        const scene::material_component& material_component = engine_ui::get().get_selected_actor()->get_component<
            scene::material_component>();

        if (material_component.get_material())
        {
            const std::shared_ptr<renderer::material>& material = material_component.get_material();
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            glm::vec4 albedo = material->get_albedo();
            if (ui_utils::draw_property("Albedo", albedo, true))
            {
                material->set_albedo(albedo);
            }

            glm::vec4 emissive = material->get_emissive();
            if (ui_utils::draw_property("Emissive", emissive, true))
            {
                material->set_emissive(emissive);
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

            float emissive_strength = material->get_emissive_strength();
            if (ui_utils::draw_property("Emissive Strength", emissive_strength, 0.0f, 10.0f, 0.01f))
            {
                material->set_emissive_strength(emissive_strength);
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

                    if (renderer::MATERIAL_TEXTURE_TYPE_SUPPORTS_CHANNEL_MODE.contains(type) &&
                        renderer::MATERIAL_TEXTURE_TYPE_SUPPORTS_CHANNEL_MODE.at(type))
                    {
                        ImGui::PushID("Channel Mode");
                        // Name
                        ImGui::AlignTextToFramePadding();
                        ImGui::Columns(2);
                        ImGui::PushItemWidth(-1);
                        ImGui::TextUnformatted("Channel Mode");
                        ImGui::NextColumn();

                        const float line_height = ImGui::GetTextLineHeightWithSpacing();
                        ImVec2 buttonSize = {line_height + 3.0f, line_height};

                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{5, 5});

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
                        if (ImGui::Button("R", buttonSize))
                        {
                            material->set_texture_channel_mode(texture.type,
                                                               renderer::material_texture_channel_mode::red);
                        }
                        ImGui::PopStyleColor(3);

                        ImGui::SameLine();

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
                        if (ImGui::Button("G", buttonSize))
                        {
                            material->set_texture_channel_mode(texture.type,
                                                               renderer::material_texture_channel_mode::green);
                        }
                        ImGui::PopStyleColor(3);

                        ImGui::SameLine();

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
                        if (ImGui::Button("B", buttonSize))
                        {
                            material->set_texture_channel_mode(texture.type,
                                                               renderer::material_texture_channel_mode::blue);
                        }
                        ImGui::PopStyleColor(3);

                        ImGui::PopStyleVar();

                        // Reset
                        ImGui::Columns(1);
                        ImGui::PopID();
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
