#include "lypch.h"
#include "shadows_pass.h"

#include "application/application.h"
#include "renderer/renderer/renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "time/app_time.h"

#include <random>

namespace luly::renderer
{
    shadows_pass::shadows_pass() : render_pass("shadows_pass")
    {
        shadows_pass::initialize();
    }

    shadows_pass::~shadows_pass()
    {
    }

    void shadows_pass::initialize()
    {
        m_directional_light_shadows_manager = std::make_shared<directional_light_shadows_manager>();
        m_point_light_shadows_manager = std::make_shared<point_light_shadows_manager>();

        initialize_shadows_data();
    }

    void shadows_pass::execute()
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        renderer::set_state(renderer_state::depth, true);

        // 1. Perform directional light cascaded shadow mapping
        m_directional_light_shadows_manager->execute(current_scene);
        // 2. Perform shadow pass on all point lights.
        m_point_light_shadows_manager->execute(current_scene);

        renderer::set_state(renderer_state::depth, false);
        renderer::set_viewport_size(renderer::get_viewport_size());
    }

    void shadows_pass::set_outputs()
    {
        clear_outputs();

        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        const std::vector<scene::directional_light_component>& directional_lights = current_scene->
            get_directional_light();
        if (directional_lights.empty()) return;

        render_pass_output directional_shadow_map_output;
        directional_shadow_map_output.name = "directional_shadow_map_output";
        directional_shadow_map_output.output = directional_lights.front().get_directional_light()->get_shadow_maps();
        add_output(directional_shadow_map_output);
    }

    void shadows_pass::on_resize(const glm::ivec2& dimensions)
    {
    }

    void shadows_pass::bind_uniforms(const std::shared_ptr<shader>& shader) const
    {
        shader->set_int("u_shadows.soft_shadows", m_shadows_data.soft_shadows);
        shader->set_int("u_shadows.pcf_horizontal_samples", m_shadows_data.pcf_horizontal_samples);
        shader->set_int("u_shadows.pcf_vertical_samples", m_shadows_data.pcf_vertical_samples);

        m_directional_light_shadows_manager->bind_uniforms(shader);
        m_point_light_shadows_manager->bind_uniforms(shader);
    }

    void shadows_pass::initialize_shadows_data()
    {
        m_shadows_data.soft_shadows = true;
        m_shadows_data.pcf_vertical_samples = 4;
        m_shadows_data.pcf_horizontal_samples = 4;
    }
}
