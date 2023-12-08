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
       for (const renderer::tonemapping_type type : renderer::TONEMAPPING_TYPES)
       {
           m_tonemapping_names.push_back(renderer::tonemapping_pass::get_tonemapping_type_to_string(type));
       }
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

            renderer::tonemapping_type tonemapping_type = renderer::scene_renderer::get_data().tonemapping_pass->get_type();
            if (ui_utils::draw_drop_down("Tone Mapping", m_tonemapping_names.data(), m_tonemapping_names.size(), (int*) &tonemapping_type))
            {
                renderer::scene_renderer::get_data().tonemapping_pass->set_type(tonemapping_type);
            }

            ui_utils::draw_property("Render Passes");
            ImGui::Separator();

            // Skybox Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().shadows_pass))
            {
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

            // Bloom Pass.
            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().bloom_pass))
            {
                ui_utils::draw_property("Mips");
                ImGui::Separator();

                ImGui::Columns(3);
                for (const renderer::bloom_pass_mip_data& bloom_mip : renderer::scene_renderer::get_data().bloom_pass->
                     get_mips_data())
                {
                    if (ui_utils::draw_property(bloom_mip.texture->get_handle_id(), {90, 90}))
                    {
                        engine_ui::set_render_target(bloom_mip.texture->get_handle_id());
                    }
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);

                ui_utils::draw_property("Parameters");
                ImGui::Separator();
                float strength = renderer::scene_renderer::get_data().bloom_pass->get_strength();
                if (ui_utils::draw_property("Strength", strength, 0.001f, 5.0f, 0.001f))
                {
                    renderer::scene_renderer::get_data().bloom_pass->set_strength(strength);
                }

                float filter_radius = renderer::scene_renderer::get_data().bloom_pass->get_filter_radius();
                if (ui_utils::draw_property("Filter Radius", filter_radius, 0.001f, 5.0f, 0.001f))
                {
                    renderer::scene_renderer::get_data().bloom_pass->set_filter_radius(filter_radius);
                }
                ImGui::TreePop();
            }

            // AO Pass
            /*
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
            */

            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().skybox_pass))
            {
                ImGui::TreePop();
            }

            ImGui::Separator();
            if (draw_render_pass_details(renderer::scene_renderer::get_data().tonemapping_pass))
            {
                float exposure = renderer::scene_renderer::get_data().tonemapping_pass->get_exposure();
                if (ui_utils::draw_property("Exposure", exposure, 0.01f, 5.0f, 0.01f))
                {
                    renderer::scene_renderer::get_data().tonemapping_pass->set_exposure(exposure);
                }

                float gamma = renderer::scene_renderer::get_data().tonemapping_pass->get_gamma();
                if (ui_utils::draw_property("Gamma", gamma, 0.01f, 5.0f, 0.01f))
                {
                    renderer::scene_renderer::get_data().tonemapping_pass->set_gamma(gamma);
                }

                ImGui::TreePop();
            }

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
            if (render_pass->get_fbo())
            {
                ui_utils::draw_property("Frame Buffer");
                ImGui::Separator();
                ui_utils::draw_property("Dimensions",
                                        std::format("{}x{}", render_pass->get_fbo()->get_width(),
                                                    render_pass->get_fbo()->get_height()));
            }

            ui_utils::draw_property("Outputs");
            ImGui::Separator();
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
