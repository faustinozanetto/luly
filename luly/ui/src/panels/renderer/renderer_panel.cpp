#include "renderer_panel.h"

#include "engine_ui.h"
#include "renderer/renderer/renderer.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/scene_manager.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    bool renderer_panel::s_show = true;

    renderer_panel::renderer_panel() : ui_panel("renderer_panel")
    {
    }

    renderer_panel::~renderer_panel()
    {
    }

    void renderer_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Renderer", &s_show))
        {
            ui_utils::draw_property("Settings");
            ImGui::Separator();
            bool vsync_enabled = renderer::renderer::get_is_vsync_enabled();
            if (ui_utils::draw_property("VSync Enabled", vsync_enabled))
            {
                renderer::renderer::set_vsync_enabled(vsync_enabled);
            }
            ImGui::Separator();
            ui_utils::draw_property("Render Passes");

            // Skybox Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().shadows_pass))
            {
                const std::shared_ptr<renderer::directional_light>& directional_light = scene::scene_manager::get().
                    get_current_scene()->get_directional_light();
                if (directional_light)
                {
                }
                ImGui::TreePop();
            }

            // Geometry Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().geometry_pass))
            {
                ImGui::TreePop();
            }

            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().environment_pass))
            {
                ImGui::TreePop();
            }

            // Lighting Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().lighting_pass))
            {
                ImGui::TreePop();
            }

            // AO Pass
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().ambient_occlusion_pass))
            {
                float bias = renderer::scene_renderer::get_data().ambient_occlusion_pass->get_ssao_bias();
                if (ui_utils::draw_property("Bias", bias, 0.001f, 1.0f, 0.001f))
                {
                    renderer::scene_renderer::get_data().ambient_occlusion_pass->set_ssao_bias(bias);
                }

                float radius = renderer::scene_renderer::get_data().ambient_occlusion_pass->get_ssao_radius();
                if (ui_utils::draw_property("Radius", radius, 0.001f, 5.0f, 0.001f))
                {
                    renderer::scene_renderer::get_data().ambient_occlusion_pass->set_ssao_radius(radius);
                }

                float noise_size = renderer::scene_renderer::get_data().ambient_occlusion_pass->get_ssao_noise_size();
                if (ui_utils::draw_property("Noise Size", noise_size, 0.01f, 10.0f, 0.01f))
                {
                    renderer::scene_renderer::get_data().ambient_occlusion_pass->set_ssao_noise_size(noise_size);
                }

                ImGui::TreePop();
            }

            // Skybox Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().skybox_pass))
            {
                ImGui::TreePop();
            }

            // Final Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().final_pass))
            {
                ImGui::TreePop();
            }

            ImGui::End();
        }
    }

    bool renderer_panel::get_show_panel()
    {
        return s_show;
    }

    void renderer_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }

    bool renderer_panel::draw_render_pass_details(const std::shared_ptr<renderer::render_pass>& render_pass)
    {
        if (ImGui::TreeNode(render_pass->get_name().c_str()))
        {
            draw_render_pass_outputs(render_pass->get_outputs());
            return true;
        }
        return false;
    }

    void renderer_panel::draw_render_pass_outputs(
        const std::unordered_map<std::string, renderer::render_pass_output>& pass_outputs)
    {
        ImGui::Columns(2);
        for (std::pair<std::string, renderer::render_pass_output> pair : pass_outputs)
        {
            if (ui_utils::draw_property(pair.second.output, {90, 90}))
            {
                engine_ui::set_render_target(pair.second.output);
            }
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
}
