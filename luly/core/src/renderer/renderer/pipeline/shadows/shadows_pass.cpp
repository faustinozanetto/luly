#include "lypch.h"
#include "shadows_pass.h"

#include "application/application.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

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

        /*
        m_point_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/point_light_shadows.lsh");
*/

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
        //  calculate_point_lights_shadows(current_scene);
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
        m_shadows_data.soft_shadows = 1;
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

    /*
        void shadows_pass::calculate_point_lights_shadows(const std::shared_ptr<scene::scene>& current_scene) const
        {
            m_point_light_shadows_shader->bind();
    
            const std::vector<std::shared_ptr<point_light>>& point_lights = current_scene->get_point_lights();
            for (const std::shared_ptr<point_light>& point_light : point_lights)
            {
                calculate_point_light_shadow(current_scene, point_light);
            }
    
            m_point_light_shadows_shader->un_bind();
            // renderer::clear_screen();
        }
    
        void shadows_pass::calculate_point_light_shadow(const std::shared_ptr<scene::scene>& current_scene,
                                                        const std::shared_ptr<point_light>& point_light) const
        {
            const std::shared_ptr<perspective_camera>& perspective_camera = current_scene->get_camera_manager()->
                get_perspective_camera();
    
            // Setup fbo and shader.
            point_light->get_shadow_map_fbo()->bind();
            renderer::set_viewport_size(point_light->get_shadow_map_dimensions());
            glClear(GL_DEPTH_BUFFER_BIT);
    
            renderer::set_cull_face_mode(renderer_cull_face_mode::front);
    
            // Update shadow transforms
            point_light->update_shadow_transforms(point_light->get_position());
    
            // Upload uniforms.
            for (uint32_t i = 0; i < point_light->get_shadow_transforms().size(); i++)
            {
                m_point_light_shadows_shader->set_mat4("u_shadow_transforms[" + std::to_string(i) + "]",
                                                       point_light->get_shadow_transforms()[i]);
            }
    
            m_point_light_shadows_shader->set_float("u_far_plane", point_light->get_shadow_map_far_plane());
            m_point_light_shadows_shader->set_vec_float3("u_light_pos", point_light->get_position());
    
            // Render geometry.
            render_geometry();
    
            // Reset state.
            renderer::set_cull_face_mode(renderer_cull_face_mode::back);
            point_light->get_shadow_map_fbo()->un_bind();
        }*/
}
