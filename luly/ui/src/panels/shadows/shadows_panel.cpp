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

            const std::shared_ptr<renderer::shadows_pass>& shadows_pass = renderer::scene_renderer::get_render_pass<
                renderer::shadows_pass>(renderer::render_pass_type::shadow_pass);

            renderer::shadows_data& shadows_data = shadows_pass->
                get_shadows_data();

            int pcf_horizontal_samples = shadows_data.pcf_horizontal_samples;
            if (ui_utils::draw_property("PCF Horizontal Samples", pcf_horizontal_samples, 1, 12))
            {
                shadows_data.pcf_horizontal_samples = pcf_horizontal_samples;
            }

            int pcf_vertical_samples = shadows_data.pcf_vertical_samples;
            if (ui_utils::draw_property("PCF Vertical Samples", pcf_vertical_samples, 1, 12))
            {
                shadows_data.pcf_vertical_samples = pcf_vertical_samples;
            }

            bool soft_shadows = shadows_data.soft_shadows;
            if (ui_utils::draw_property("Soft Shadows", soft_shadows))
            {
                shadows_data.soft_shadows = soft_shadows;
            }

            if (ImGui::TreeNode("Cascaded Shadows"))
            {
                const std::shared_ptr<renderer::directional_light_shadows_manager>& directional_light_shadows_manager =
                    shadows_pass->get_directional_light_shadows_manager();

                const renderer::directional_light_shadows_data& directional_light_shadows_data =
                    directional_light_shadows_manager->get_directional_light_shadows_data();

                float shadow_bias = directional_light_shadows_data.shadow_bias;
                if (ui_utils::draw_property("Shadow Bias", shadow_bias, 0.0f, 5.0f, 0.0001f))
                {
                    directional_light_shadows_manager->set_bias(shadow_bias);
                }

                bool show_cascades = directional_light_shadows_data.show_cascades;
                if (ui_utils::draw_property("Show Cascades", show_cascades))
                {
                    directional_light_shadows_manager->set_show_cascades(show_cascades);
                }

                for (int i = 0; i < CASCADES_COUNT; i++)
                {
                    glm::vec4 cascade_debug_color = directional_light_shadows_data.cascade_debug_colors[i];
                    if (ui_utils::draw_property(std::format("Cascade #{} Debug Color", i + 1), cascade_debug_color))
                    {
                        directional_light_shadows_manager->set_debug_cascade_color(i, cascade_debug_color);
                    }
                }

                ImGui::TreePop();
            }
        }
        ImGui::End();
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
