#include "renderer_panel.h"

#include "engine_ui.h"
#include "renderer/renderer/renderer.h"
#include "renderer/scene/scene_renderer.h"
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

            // Geometry Pass.
            ImGui::Separator();
            draw_render_pass_details(renderer::scene_renderer::get_data().geometry_pass);

            ImGui::Separator();
            if (ImGui::TreeNode("Environment Pass"))
            {
                const std::shared_ptr<renderer::environment_pass>& environment_pass =
                    renderer::scene_renderer::get_data().environment_pass;

                const renderer::render_pass_output& brdf_output = environment_pass->get_output("brdf_output");
                if (ui_utils::draw_property(brdf_output.pass_output->get_handle_id(), {90, 90}))
                {
                    engine_ui::set_render_target(brdf_output.pass_output->get_handle_id());
                }
                ImGui::TreePop();
            }

            // Lighting Pass.
            ImGui::Separator();
            draw_render_pass_details(renderer::scene_renderer::get_data().lighting_pass);

            // Skybox Pass.
            ImGui::Separator();
            draw_render_pass_details(renderer::scene_renderer::get_data().skybox_pass);

            // Final Pass.
            ImGui::Separator();
            draw_render_pass_details(renderer::scene_renderer::get_data().final_pass);

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

    void renderer_panel::draw_render_pass_details(const std::shared_ptr<renderer::render_pass>& render_pass)
    {
        if (ImGui::TreeNode(render_pass->get_name().c_str()))
        {
            const std::shared_ptr<renderer::frame_buffer>& pass_fbo = render_pass->get_frame_buffer();
            draw_render_pass_fbo_attachments(pass_fbo);

            ImGui::TreePop();
        }
    }

    void renderer_panel::draw_render_pass_fbo_attachments(const std::shared_ptr<renderer::frame_buffer>& frame_buffer)
    {
        ImGui::Columns(2);
        for (uint32_t attachment_handle : frame_buffer->get_attachments())
        {
            if (ui_utils::draw_property(attachment_handle, {90, 90}))
            {
                engine_ui::set_render_target(attachment_handle);
            }
            ImGui::NextColumn();
        }
        ImGui::Columns(1);
    }
}
