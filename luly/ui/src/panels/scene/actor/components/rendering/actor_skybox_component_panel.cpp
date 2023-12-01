﻿#include "actor_skybox_component_panel.h"

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
        bool has_component = engine_ui::get_ui_data().selected_actor->has_component<scene::skybox_component>();
        auto component_hash = typeid(scene::skybox_component).hash_code();
        return std::make_pair(has_component, component_hash);
    }

    void actor_skybox_component_panel::on_render_component_details()
    {
        auto& skybox_component = engine_ui::get_ui_data().selected_actor->get_component<
            scene::skybox_component>();

        if (skybox_component.get_environment_texture())
        {
            ui_utils::draw_property("Parameters");
            ImGui::Separator();

            if (ui_utils::draw_property("Environemt Map", skybox_component.get_environment_texture()))
            {
                std::string file_path = utils::file_utils::open_file_dialog("Environment Map", {"*.hdr"});
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
            if (ui_utils::draw_property("Intesity", intensity, 0.0f, 5.0f, 0.01f))
            {
                skybox_component.set_intensity(intensity);
            }
        }
        else
        {
            ImGui::Text("No model currently loaded!");
        }
    }
}
