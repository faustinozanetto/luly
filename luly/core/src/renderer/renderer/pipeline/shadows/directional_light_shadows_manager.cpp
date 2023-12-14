#include "lypch.h"
#include "directional_light_shadows_manager.h"

#include "scene/actor/components/lights/directional_light_component.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    directional_light_shadows_manager::directional_light_shadows_manager()
    {
        initialize();
    }

    void directional_light_shadows_manager::set_debug_cascade_color(int cascade_index, const glm::vec3& color)
    {
        LY_PROFILE_FUNCTION;
        LY_ASSERT_MSG(cascade_index >= 0 && cascade_index < CASCADES_COUNT, "Cascade index out of range!")
        m_directional_light_shadows_data.cascade_debug_colors[cascade_index] = glm::vec4(color, 1.0);
    }

    void directional_light_shadows_manager::execute(const std::shared_ptr<scene::scene>& current_scene)
    {
        LY_PROFILE_FUNCTION;
        const std::vector<scene::directional_light_component>& directional_lights = current_scene->
            get_directional_light();
        if (directional_lights.empty()) return;

        const scene::directional_light_component& directional_light = directional_lights.front();
        const std::shared_ptr<perspective_camera>& perspective_camera = current_scene->get_camera_manager()->
            get_perspective_camera();

        // Check if light has shadow mapping enable.
        const bool enable_shadows = directional_light.get_enable_shadows();
        m_directional_light_shadows_data.enable_shadows = enable_shadows;

        if (!enable_shadows) return;

        // Calculate cascades and update ubo.
        directional_light.get_directional_light()->update_shadow_cascades(perspective_camera);
        update_shadows_cascades_data(directional_light.get_directional_light());

        // Bind fbo and shader.
        directional_light.get_directional_light()->get_shadow_map_fbo()->bind();
        renderer::set_viewport_size(directional_light.get_directional_light()->get_shadow_map_dimensions());
        glClear(GL_DEPTH_BUFFER_BIT);

        // Bind shader
        renderer::set_cull_face_mode(renderer_cull_face_mode::front);
        m_directional_light_shadows_shader->bind();

        // Render geometry.
        render_geometry(m_directional_light_shadows_shader);

        // Reset state.
        renderer::set_cull_face_mode(renderer_cull_face_mode::back);
        directional_light.get_directional_light()->get_shadow_map_fbo()->un_bind();
        m_directional_light_shadows_shader->un_bind();
    }

    void directional_light_shadows_manager::bind_uniforms(const std::shared_ptr<shader>& shader)
    {
        LY_PROFILE_FUNCTION;
        shader->set_int("u_directional_light_shadows.show_cascades", m_directional_light_shadows_data.show_cascades);
        shader->set_float("u_directional_light_shadows.shadow_bias", m_directional_light_shadows_data.shadow_bias);
        shader->set_int("u_directional_light_shadows.enable_shadows", m_directional_light_shadows_data.enable_shadows);
        for (int i = 0; i < CASCADES_COUNT; i++)
        {
            shader->set_float("u_directional_light_shadows.cascade_plane_distances[" + std::to_string(i) + "]",
                              m_directional_light_shadows_data.cascade_plane_distances[i]);
            shader->set_vec_float4("u_directional_light_shadows.cascade_debug_colors[" + std::to_string(i) + "]",
                                   m_directional_light_shadows_data.cascade_debug_colors[i]);
        }
    }

    void directional_light_shadows_manager::initialize()
    {
        LY_PROFILE_FUNCTION;
        m_directional_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/directional_light_shadows.lsh");

        initialize_shadows_data();
    }

    void directional_light_shadows_manager::initialize_shadows_data()
    {
        LY_PROFILE_FUNCTION;
        m_directional_light_shadows_data.show_cascades = false;
        m_directional_light_shadows_data.enable_shadows = false;
        m_directional_light_shadows_data.shadow_bias = 0.005f;
        m_directional_light_shadows_data.cascade_debug_colors[0] = glm::vec4(0.92f, 0.92f, 0.1f, 1);
        m_directional_light_shadows_data.cascade_debug_colors[1] = glm::vec4(0.1f, 0.83f, 0.82f, 1);
        m_directional_light_shadows_data.cascade_debug_colors[2] = glm::vec4(0.87f, 0.12f, 0.62f, 1);
        m_directional_light_shadows_data.cascade_plane_distances[0] = 0.0f;
        m_directional_light_shadows_data.cascade_plane_distances[1] = 0.0f;
        m_directional_light_shadows_data.cascade_plane_distances[2] = 0.0f;
    }

    void directional_light_shadows_manager::update_shadows_cascades_data(
        const std::shared_ptr<directional_light>& directional_light)
    {
        LY_PROFILE_FUNCTION;
        const std::vector<float>& cascade_splits = directional_light->get_shadow_cascade_splits();
        for (int i = 0; i < CASCADES_COUNT; i++)
        {
            m_directional_light_shadows_data.cascade_plane_distances[i] = cascade_splits[i];
        }
    }
}
