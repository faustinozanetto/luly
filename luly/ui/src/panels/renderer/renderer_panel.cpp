#include "renderer_panel.h"

#include "engine_ui.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/pipeline/bloom_pass.h"
#include "renderer/renderer/pipeline/tonemapping_pass.h"
#include "renderer/scene/scene_renderer.h"
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
            ui_utils::draw_property("Common");
            ImGui::Separator();

            bool vsync_enabled = renderer::renderer::get_is_vsync_enabled();
            if (ui_utils::draw_property("VSync Enabled", vsync_enabled))
            {
                renderer::renderer::set_vsync_enabled(vsync_enabled);
            }

            render_tonemapping_details();
            render_bloom_details();

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

    void renderer_panel::render_tonemapping_details()
    {
        const std::shared_ptr<renderer::tonemapping_pass>& tonemapping_pass = renderer::scene_renderer::get_render_pass<
            renderer::tonemapping_pass>(renderer::render_pass_type::tonemapping_pass);

        ui_utils::draw_property("Tone Mapping");
        ImGui::Separator();

        renderer::tonemapping_type tonemapping_type = tonemapping_pass->get_type();
        if (ui_utils::draw_drop_down("Type", m_tonemapping_names.data(), m_tonemapping_names.size(),
                                     reinterpret_cast<int*>(&tonemapping_type)))
        {
            tonemapping_pass->set_type(tonemapping_type);
        }

        float exposure = tonemapping_pass->get_exposure();
        if (ui_utils::draw_property("Exposure", exposure, 0.01f, 5.0f, 0.01f))
        {
            tonemapping_pass->set_exposure(exposure);
        }

        float gamma = tonemapping_pass->get_gamma();
        if (ui_utils::draw_property("Gamma", gamma, 0.01f, 5.0f, 0.01f))
        {
            tonemapping_pass->set_gamma(gamma);
        }
    }

    void renderer_panel::render_bloom_details()
    {
        const std::shared_ptr<renderer::bloom_pass>& bloom_pass = renderer::scene_renderer::get_render_pass<
            renderer::bloom_pass>(renderer::render_pass_type::bloom_pass);

        ui_utils::draw_property("Bloom");
        ImGui::Separator();

        float strength = bloom_pass->get_strength();
        if (ui_utils::draw_property("Strength", strength, 0.001f, 5.0f, 0.001f))
        {
            bloom_pass->set_strength(strength);
        }

        float filter_radius = bloom_pass->get_filter_radius();
        if (ui_utils::draw_property("Filter Radius", filter_radius, 0.001f, 5.0f, 0.001f))
        {
            bloom_pass->set_filter_radius(filter_radius);
        }
    }
}
