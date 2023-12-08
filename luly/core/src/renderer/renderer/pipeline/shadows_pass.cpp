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

    void shadows_pass::set_debug_cascade_color(int cascade_index, const glm::vec3& color)
    {
        LY_ASSERT_MSG(cascade_index >= 0 && cascade_index < 3, "Cascade index out of range!")
        m_cascaded_shadows_parameters.cascade_debug_colors[cascade_index] = glm::vec4(color, 1.0);
    }

    void shadows_pass::initialize()
    {
        // Load shader.
        m_directional_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/directional_light_shadows.lsh");

        // Create cascaded shadows ubo.
        initialize_cascaded_shadows_parameters();
        generate_random_angles_texture();
        m_cascaded_shadows_ubo = std::make_shared<uniform_buffer_object>(sizeof(cascaded_shadows_parameters), 4);
    }

    void shadows_pass::execute()
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        // 1. Perform directional light cascaded shadow mapping
        const std::shared_ptr<directional_light>& directional_light = current_scene->get_directional_light();
        if (!directional_light) return;

        calculate_directional_light_shadows(current_scene, directional_light);
        /*
                glBindVertexArray(m_dummy_vao_id);
                glBindTextureUnit(0, directional_light->get_shadow_maps());
                m_cascades_debug_shader->bind();
        
                for(uint32_t i = 0; i < directional_light->get_cascades_count(); ++i)
                {
                    static uint32_t width = renderer::get_viewport_size().x * 0.4;
                    glViewport(width * i, 0, width, width);
                    m_cascades_debug_shader->set_int("u_cascade_index", int(i));
                    glDrawArrays(GL_TRIANGLES, 0, 3);
                }
                
                /*
                glCullFace(GL_FRONT);
                glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
        
                renderer::set_state(renderer_state::depth, true);
        
                const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        
                // 1. Render depth of scene from lights perspective.
                const auto& registry = current_scene->get_registry();
                const auto& view = registry->view<scene::point_light_component>();
                m_point_light_shadows_shader->bind();
        
                for (const auto& [actor, point_light_component] : view.each())
                {
                    const std::shared_ptr<point_light>& point_light = point_light_component.get_point_light();
        
                    glBindFramebuffer(GL_FRAMEBUFFER, point_light->get_shadow_map_fbo());
                    glClear(GL_DEPTH_BUFFER_BIT);
        
                    point_light->update_shadow_transforms(point_light->get_position());
        
                    for (uint32_t i = 0; i < point_light->get_shadow_transforms().size(); i++)
                    {
                        m_point_light_shadows_shader->set_mat4("u_shadow_transforms[" + std::to_string(i) + "]",
                                                               point_light->get_shadow_transforms()[i]);
                    }
        
                    m_point_light_shadows_shader->set_float("u_far_plane", point_light->get_shadow_map_far_plane());
                    m_point_light_shadows_shader->set_vec_float3("u_light_pos", point_light->get_position());
        
                    render_geometry();
                }
                m_point_light_shadows_shader->un_bind();
        
                renderer::set_viewport_size(renderer::get_viewport_size());
                glCullFace(GL_BACK);
                */
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

    void shadows_pass::initialize_cascaded_shadows_parameters()
    {
        m_cascaded_shadows_parameters.shadow_bias = 0.005f;
        m_cascaded_shadows_parameters.soft_shadows = 1;
        m_cascaded_shadows_parameters.pcf_vertical_samples = 4;
        m_cascaded_shadows_parameters.pcf_horizontal_samples = 4;
        m_cascaded_shadows_parameters.show_cascades = 0;
        m_cascaded_shadows_parameters.cascade_debug_colors[0] = glm::vec4(0.92f, 0.92f, 0.1f, 1);
        m_cascaded_shadows_parameters.cascade_debug_colors[1] = glm::vec4(0.1f, 0.83f, 0.82f, 1);
        m_cascaded_shadows_parameters.cascade_debug_colors[2] = glm::vec4(0.87f, 0.12f, 0.62f, 1);
        for (int i = 0; i < 3; i++)
        {
            m_cascaded_shadows_parameters.cascade_plane_distances[i] = 0.0f;
        }
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

    void shadows_pass::update_cascaded_shadows_ubo(const std::shared_ptr<directional_light>& directional_light)
    {
        const std::vector<float>& cascade_splits = directional_light->get_shadow_cascade_splits();
        for (int i = 0; i < directional_light->get_cascades_count(); i++)
        {
            m_cascaded_shadows_parameters.cascade_plane_distances[i] = cascade_splits[i];
        }
        m_cascaded_shadows_ubo->set_data(&m_cascaded_shadows_parameters, sizeof(m_cascaded_shadows_parameters));
    }

    void shadows_pass::calculate_directional_light_shadows(const std::shared_ptr<scene::scene>& current_scene,
                                                           const std::shared_ptr<directional_light>& directional_light)
    {
        renderer::set_state(renderer_state::depth, true);

        const std::shared_ptr<perspective_camera>& perspective_camera = current_scene->get_camera_manager()->
            get_perspective_camera();

        // Setup fbo and shader.
        directional_light->get_shadow_map_fbo()->bind();
        renderer::set_viewport_size(directional_light->get_shadow_map_dimensions());
        glClear(GL_DEPTH_BUFFER_BIT);

        renderer::set_cull_face_mode(renderer_cull_face_mode::front);
        m_directional_light_shadows_shader->bind();

        // Calculate cascades levels and update cascades.
        directional_light->update_shadow_cascades(perspective_camera);
        update_cascaded_shadows_ubo(directional_light);

        // Render geometry.
        render_geometry();

        // Reset state.
        renderer::set_cull_face_mode(renderer_cull_face_mode::back);
        directional_light->get_shadow_map_fbo()->un_bind();
        m_directional_light_shadows_shader->un_bind();

        // Reset
        renderer::set_viewport_size(renderer::get_viewport_size());
        renderer::clear_screen();
        renderer::set_state(renderer_state::depth, false);
    }

    void shadows_pass::render_geometry() const
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        const auto& registry = current_scene->get_registry();
        const auto& view = registry->view<scene::transform_component, scene::model_renderer_component>();

        for (auto [actor, transform_component, model_renderer_component] : view.each())
        {
            const std::shared_ptr<math::transform>& transform = transform_component.get_transform();
            const std::shared_ptr<model>& model = model_renderer_component.get_model();

            if (!model_renderer_component.get_casts_shadows()) continue;

            m_directional_light_shadows_shader->set_mat4("u_transform", transform->get_transform());

            renderer::submit_model(model);
        }
    }
}
