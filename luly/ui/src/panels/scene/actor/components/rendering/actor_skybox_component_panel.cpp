#include "actor_skybox_component_panel.h"

#include "engine_ui.h"
#include "renderer/textures/texture_factory.h"
#include "scene/actor/components/rendering/skybox_component.h"
#include "utils/file_utils.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    actor_skybox_component_panel::actor_skybox_component_panel() : actor_component_panel(
        "Skybox Component")
    {
    }

    std::pair<bool, size_t> actor_skybox_component_panel::get_actor_component_details()
    {
        bool has_component = engine_ui::get().get_selected_actor()->has_component<scene::skybox_component>();
        auto component_hash = typeid(scene::skybox_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_skybox_component_panel::on_render_component_details()
    {
        scene::skybox_component& skybox_component = engine_ui::get().get_selected_actor()->get_component<
            scene::skybox_component>();

        if (skybox_component.get_environment_texture())
        {
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            if (ui_utils::draw_property("Environment Map", skybox_component.get_environment_texture()))
            {
                const std::string file_path = utils::file_utils::open_file_dialog("Environment Map", {"*.hdr"});
                if (file_path.empty())
                {
                    LY_WARN("Failed to load environment map from file dialog!");
                }
                else
                {
                    const auto& new_environment_map = renderer::texture_factory::create_environment_texture_from_file(
                        file_path);
                    skybox_component.set_environment_texture(new_environment_map);
                }
            }

            float intensity = skybox_component.get_intensity();
            if (ui_utils::draw_property("Intensity", intensity, 0.0f, 5.0f, 0.01f))
            {
                skybox_component.set_intensity(intensity);
            }

            float lod_level = skybox_component.get_lod_level();
            if (ui_utils::draw_property("LOD Level", lod_level, 0,
                                        glm::log2(
                                            static_cast<float>(skybox_component.get_environment_texture()->
                                                get_width())), 0.01f))
            {
                skybox_component.set_lod_level(lod_level);
            }

            glm::vec3 tint = skybox_component.get_tint();
            if (ui_utils::draw_property("Tint", tint))
            {
                skybox_component.set_tint(tint);
            }
        }
        else
        {
            ImGui::Text("No model currently loaded!");
        }
    }
}
