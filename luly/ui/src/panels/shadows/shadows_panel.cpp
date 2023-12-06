#include "shadows_panel.h"

#include "engine_ui.h"
#include "renderer/scene/scene_renderer.h"
#include "utils/ui_utils.h"

namespace luly::ui
{
    bool shadows_panel::s_show = true;

    shadows_panel::shadows_panel() : ui_panel("shadows_panel")
    {
    }

    shadows_panel::~shadows_panel()
    {
    }

    void shadows_panel::on_render_panel()
    {
        if (!s_show) return;

        if (ImGui::Begin("Shadows", &s_show))
        {
            ui_utils::draw_property("Settings");
            ImGui::Separator();

            const std::shared_ptr<renderer::shadows_pass>& shadows_pass = renderer::scene_renderer::get_data().
                shadows_pass;
            renderer::cascaded_shadows_parameters& cascaded_shadows_parameters = shadows_pass->
                get_cascaded_shadows_parameters();

            float shadow_bias = cascaded_shadows_parameters.shadow_bias;
            if (ui_utils::draw_property("Shadow Bias", shadow_bias, 0.0f, 5.0f, 0.00001f))
            {
                cascaded_shadows_parameters.shadow_bias = shadow_bias;
            }

            int pcf_horizontal_samples = cascaded_shadows_parameters.pcf_horizontal_samples;
            if (ui_utils::draw_property("PCF Horizontal Samples", pcf_horizontal_samples, 1, 12))
            {
                cascaded_shadows_parameters.pcf_horizontal_samples = pcf_horizontal_samples;
            }

            int pcf_vertical_samples = cascaded_shadows_parameters.pcf_vertical_samples;
            if (ui_utils::draw_property("PCF Vertical Samples", pcf_vertical_samples, 1, 12))
            {
                cascaded_shadows_parameters.pcf_vertical_samples = pcf_vertical_samples;
            }

            bool soft_shadows = cascaded_shadows_parameters.soft_shadows;
            if (ui_utils::draw_property("Soft Shadows", soft_shadows))
            {
                cascaded_shadows_parameters.soft_shadows = soft_shadows;
            }

            if(ImGui::TreeNode("Cascade Shadows"))
            {
                bool show_cascades = cascaded_shadows_parameters.show_cascades;
                if (ui_utils::draw_property("Show Cascades", show_cascades))
                {
                    shadows_pass->set_show_cascades(show_cascades);
                }

                for (int i = 0; i < 3; i++)
                {
                    glm::vec4 cascade_debug_color = cascaded_shadows_parameters.cascade_debug_colors[i];
                    if (ui_utils::draw_property(std::format("Cascade #{} Debug Color", i + 1), cascade_debug_color))
                    {
                        shadows_pass->set_debug_cascade_color(i, cascade_debug_color);
                    }
                }

                ImGui::TreePop();
            }

            ImGui::End();
        }
    }

    bool shadows_panel::get_show_panel()
    {
        return s_show;
    }

    void shadows_panel::set_show_panel(bool show_panel)
    {
        s_show = show_panel;
    }
}
