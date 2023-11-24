#include "renderer_panel.h"

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
            
            ImGui::Separator();
            if (ImGui::TreeNode("Geometry Pass"))
            {
                auto& geometry_pass = renderer::scene_renderer::get_data().geometry_pass;
                auto& geometry_fbo = geometry_pass->get_frame_buffer();
                ImGui::Columns(2);
                for (uint32_t attachment_handle : geometry_fbo->get_attachments())
                {
                    ui_utils::draw_property(attachment_handle, {90, 90});
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
                ImGui::TreePop();
            }

            ImGui::Separator();
            if (ImGui::TreeNode("Lighting Pass"))
            {
                auto& lighting_pass = renderer::scene_renderer::get_data().lighting_pass;
                auto& lighting_fbo = lighting_pass->get_frame_buffer();
                ImGui::Columns(2);
                for (uint32_t attachment_handle : lighting_fbo->get_attachments())
                {
                    ui_utils::draw_property(attachment_handle, {90, 90});
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
                ImGui::TreePop();
            }

            ImGui::Separator();
            if (ImGui::TreeNode("Final Pass"))
            {
                auto& final_pass = renderer::scene_renderer::get_data().final_pass;
                auto& final_fbo = final_pass->get_frame_buffer();
                ImGui::Columns(2);
                for (uint32_t attachment_handle : final_fbo->get_attachments())
                {
                    ui_utils::draw_property(attachment_handle, {90, 90});
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
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
}
