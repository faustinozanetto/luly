#include "render_passes_panel.h"

#include "engine_ui.h"
#include "renderer/renderer/renderer.h"
#include "renderer/scene/scene_renderer.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    bool render_passes_panel::s_show = true;

    render_passes_panel::render_passes_panel() : ui_panel("render_passes_panel")
    {
    }

    render_passes_panel::~render_passes_panel()
    {
    }

    void render_passes_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Render Passes", &s_show))
        {
            /*
        	if (draw_render_pass_details(renderer::scene_renderer::get_data().shadows_pass))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(renderer::scene_renderer::get_data().geometry_pass))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(renderer::scene_renderer::get_data().lighting_pass))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(renderer::scene_renderer::get_data().skybox_pass))
            {
                ImGui::TreePop();
            }
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
                        engine_ui::get().set_render_target(bloom_mip.texture->get_handle_id());
                    }
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(renderer::scene_renderer::get_data().tonemapping_pass))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(renderer::scene_renderer::get_data().debanding_pass))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(renderer::scene_renderer::get_data().final_pass))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();*/

            ImGui::End();
        }
    }

    bool render_passes_panel::get_show_panel()
    {
        return s_show;
    }

    void render_passes_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }

    bool render_passes_panel::draw_render_pass_details(const std::shared_ptr<renderer::render_pass>& render_pass)
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

    void render_passes_panel::draw_render_pass_outputs(
        const std::unordered_map<std::string, renderer::render_pass_output>& pass_outputs)
    {
        ImGui::Columns(2);
        for (std::pair<std::string, renderer::render_pass_output> pair : pass_outputs)
        {
            if (ui_utils::draw_property(pair.second.output, {90, 90}))
            {
                engine_ui::get().set_render_target(pair.second.output);
            }
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
}
