#include "lypch.h"
#include "shadows_pass.h"

#include "application/application.h"
#include "renderer/renderer/renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"

#include <random>

#include "time/app_time.h"

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
        generate_random_angles_texture();
        m_shadows_ubo = std::make_shared<uniform_buffer_object>(sizeof(shadows_data), SHADOWS_UBO_LOCATION);
    }

    void shadows_pass::execute()
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        renderer::set_state(renderer_state::depth, true);
        update_shadows_ubo();

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
        const std::shared_ptr<directional_light>& directional_light = current_scene->get_directional_light();

        render_pass_output directional_shadow_map_output;
        directional_shadow_map_output.name = "directional_shadow_map_output";
        directional_shadow_map_output.output = directional_light->get_shadow_maps();
        add_output(directional_shadow_map_output);

        render_pass_output random_angles_map_output;
        random_angles_map_output.name = "random_angles_map_output";
        random_angles_map_output.output = m_random_angles_texture->get_handle_id();
        add_output(random_angles_map_output);
    }

    void shadows_pass::on_resize(const glm::ivec2& dimensions)
    {
    }

    void shadows_pass::initialize_shadows_data()
    {
        m_shadows_data.soft_shadows = true;
        m_shadows_data.pcf_vertical_samples = 4;
        m_shadows_data.pcf_horizontal_samples = 4;
    }

    void shadows_pass::update_shadows_ubo() const
    {
        m_shadows_ubo->set_data(&m_shadows_data, sizeof(m_shadows_data));
    }

    void shadows_pass::generate_random_angles_texture()
    {
        std::uniform_real_distribution<double> distribution(0.0f, 1.0f);
        std::mt19937 generator(app_time::get_time());

        const int size = 64;
        std::vector<glm::vec2> data;
        for (int y = 0; y < size; ++y)
        {
            for (int x = 0; x < size; ++x)
            {
                const float random_angle = glm::two_pi<float>() * distribution(generator);
                // Random angles in range [0, 2PI);
                data.emplace_back(glm::cos(random_angle), glm::sin(random_angle));
            }
        }

        texture_specification random_angles_texture_specification = {
            size, size, 2, data.data(), texture_internal_format::rg32f
        };
        m_random_angles_texture = std::make_shared<texture_2d>(random_angles_texture_specification);
    }
}
