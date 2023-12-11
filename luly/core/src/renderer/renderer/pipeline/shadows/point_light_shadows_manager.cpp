#include "lypch.h"
#include "point_light_shadows_manager.h"

#include "renderer/renderer/pipeline/lighting_pass.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    point_light_shadows_manager::point_light_shadows_manager()
    {
        initialize();
    }

    void point_light_shadows_manager::execute(const std::shared_ptr<scene::scene>& current_scene)
    {
        const std::vector<std::shared_ptr<point_light>>& point_lights = current_scene->get_point_lights();
        if (point_lights.empty()) return;

        m_point_light_shadows_shader->bind();
        int light_index = 0;
        for (const std::shared_ptr<point_light>& point_light : point_lights)
        {
            m_point_light_shadows_data.far_planes[light_index++] = point_light->get_shadow_map_far_plane();

            calculate_point_light_shadow(point_light);
        }
        m_point_light_shadows_shader->un_bind();
    }

    void point_light_shadows_manager::bind_uniforms(const std::shared_ptr<shader>& shader)
    {
        shader->set_float("u_point_light_shadows.shadow_bias", m_point_light_shadows_data.shadow_bias);
        for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        {
            shader->set_float("u_point_light_shadows.far_planes[" + std::to_string(i) + "]",
                              m_point_light_shadows_data.far_planes[i]);
        }
    }

    void point_light_shadows_manager::initialize()
    {
        m_point_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/point_light_shadows.lsh");

        initialize_shadows_data();
    }

    void point_light_shadows_manager::initialize_shadows_data()
    {
        m_point_light_shadows_data.shadow_bias = 0.005f;
        for (int i = 0; i < MAX_POINT_LIGHTS; i++)
        {
            m_point_light_shadows_data.far_planes[i] = 0.0f;
        }
    }

    void point_light_shadows_manager::calculate_point_light_shadow(const std::shared_ptr<point_light>& point_light)
    {
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
        render_geometry(m_point_light_shadows_shader);

        // Reset state.
        renderer::set_cull_face_mode(renderer_cull_face_mode::back);
        point_light->get_shadow_map_fbo()->un_bind();
    }
}
