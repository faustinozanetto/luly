#include "lypch.h"
#include "directional_light_shadows_manager.h"

#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    directional_light_shadows_manager::directional_light_shadows_manager()
    {
        initialize();
    }

    void directional_light_shadows_manager::set_debug_cascade_color(int cascade_index, const glm::vec3& color)
    {
        LY_ASSERT_MSG(cascade_index >= 0 && cascade_index < CASCADES_COUNT, "Cascade index out of range!")
        m_directional_light_shadows_data.cascade_debug_colors[cascade_index] = glm::vec4(color, 1.0);
    }

    void directional_light_shadows_manager::execute(const std::shared_ptr<scene::scene>& current_scene)
    {
        const std::shared_ptr<directional_light>& directional_light = current_scene->get_directional_light();
        if (!directional_light) return;

        const std::shared_ptr<perspective_camera>& perspective_camera = current_scene->get_camera_manager()->
            get_perspective_camera();

        // Bind fbo and shader.
        directional_light->get_shadow_map_fbo()->bind();
        renderer::set_viewport_size(directional_light->get_shadow_map_dimensions());
        glClear(GL_DEPTH_BUFFER_BIT);

        // Bind shader
        renderer::set_cull_face_mode(renderer_cull_face_mode::front);
        m_directional_light_shadows_shader->bind();

        // Calculate cascades and update ubo.
        directional_light->update_shadow_cascades(perspective_camera);
        update_shadows_ubo(directional_light);

        // Render geometry.
        render_geometry(m_directional_light_shadows_shader);

        // Reset state.
        renderer::set_cull_face_mode(renderer_cull_face_mode::back);
        directional_light->get_shadow_map_fbo()->un_bind();
        m_directional_light_shadows_shader->un_bind();
    }

    void directional_light_shadows_manager::initialize()
    {
        m_directional_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/directional_light_shadows.lsh");

        initialize_shadows_data();

        m_directional_light_shadows_ubo  = std::make_shared<uniform_buffer_object>(
            sizeof(m_directional_light_shadows_data), DIRECTIONAL_LIGHT_SHADOWS_UBO_LOCATION);
    }

    void directional_light_shadows_manager::initialize_shadows_data()
    {
        m_directional_light_shadows_data.show_cascades = 0;
        m_directional_light_shadows_data.shadow_bias = 0.005f;
        m_directional_light_shadows_data.cascade_debug_colors[0] = glm::vec4(0.92f, 0.92f, 0.1f, 1);
        m_directional_light_shadows_data.cascade_debug_colors[1] = glm::vec4(0.1f, 0.83f, 0.82f, 1);
        m_directional_light_shadows_data.cascade_debug_colors[2] = glm::vec4(0.87f, 0.12f, 0.62f, 1);
        m_directional_light_shadows_data.cascade_plane_distances[0] = 0.0f;
        m_directional_light_shadows_data.cascade_plane_distances[1] = 0.0f;
        m_directional_light_shadows_data.cascade_plane_distances[2] = 0.0f;
    }

    void directional_light_shadows_manager::update_shadows_ubo(
        const std::shared_ptr<directional_light>& directional_light)
    {
        const std::vector<float>& cascade_splits = directional_light->get_shadow_cascade_splits();
        for (int i = 0; i < directional_light->get_cascades_count(); i++)
        {
            m_directional_light_shadows_data.cascade_plane_distances[i] = cascade_splits[i];
        }
        m_directional_light_shadows_ubo->set_data(&m_directional_light_shadows_data,
                                                  sizeof(m_directional_light_shadows_data));
    }
}
