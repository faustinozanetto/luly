#include "render_passes_panel.h"

#include "engine_ui.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/pipeline/bloom_pass.h"
#include "renderer/renderer/pipeline/debanding_pass.h"
#include "renderer/renderer/pipeline/debug_pass.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/renderer/pipeline/skybox_pass.h"
#include "renderer/renderer/pipeline/tonemapping_pass.h"
#include "renderer/renderer/pipeline/lighting/lighting_pass.h"
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
            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::shadows_pass>(
                    renderer::render_pass_type::shadow_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::geometry_pass>(
                    renderer::render_pass_type::geometry_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::lighting_pass>(
                    renderer::render_pass_type::lighting_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::skybox_pass>(
                    renderer::render_pass_type::skybox_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            const std::shared_ptr<renderer::bloom_pass>& bloom_pass = renderer::scene_renderer::get_render_pass<
                renderer::bloom_pass>(
                renderer::render_pass_type::bloom_pass);
            if (draw_render_pass_details(bloom_pass))
            {
                ui_utils::draw_property("Mips");
                ImGui::Separator();

                ImGui::Columns(3);
                for (const renderer::bloom_pass_mip_data& bloom_mip : bloom_pass->get_mips_data())
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

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::tonemapping_pass>(
                    renderer::render_pass_type::tonemapping_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::debanding_pass>(
                    renderer::render_pass_type::debanding_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::debug_pass>(
                    renderer::render_pass_type::debug_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();

            if (draw_render_pass_details(
                renderer::scene_renderer::get_render_pass<renderer::final_pass>(
                    renderer::render_pass_type::final_pass)))
            {
                ImGui::TreePop();
            }
            ImGui::Separator();
        }
        ImGui::End();
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

        for (const auto& render_pass : pass_outputs)
        {
            // Draw the name above the image
            ImGui::Text("%s", render_pass.second.name.c_str());

            // Align the image to the center of the column
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() - 90) / 2);

            // Draw the image
            if (ui_utils::draw_property(render_pass.second.output, {90, 90}))
            {
                engine_ui::get().set_render_target(render_pass.second.output);
            }

            // Move to the next column
            ImGui::NextColumn();
        }

        // Reset column layout
        ImGui::Columns(1);
    }
}
