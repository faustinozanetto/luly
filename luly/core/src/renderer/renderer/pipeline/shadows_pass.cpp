#include "lypch.h"
#include "shadows_pass.h"

#include "application/application.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

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
        // Load shader.
        m_directional_light_shadows_shader = shader_factory::create_shader_from_file(
            "assets/shaders/shadows/directional_light_shadows.lsh");
    }

    void shadows_pass::execute()
    {
        calculate_directional_light_shadows();
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
    }

    void shadows_pass::calculate_directional_light_shadows()
    {
        renderer::set_state(renderer_state::depth, true);

        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        // 1. Perform directional light cascaded shadow mapping
        const std::shared_ptr<directional_light>& directional_light = current_scene->get_directional_light();
        if (!directional_light) return;

        directional_light->update_shadow_map_views();
        
        // Calculate cascades levels and update cascades.
        directional_light->calculate_shadow_map_levels(
            current_scene->get_camera_manager()->get_perspective_camera()->get_far_clip());
        // directional_light->update_shadow_cascades(current_scene->get_camera_manager()->get_perspective_camera());
        std::vector<glm::mat4> lightSpaceMatrices = directional_light->get_light_space_matrices(
            current_scene->get_camera_manager()->get_perspective_camera());
        for (size_t i = 0; i < lightSpaceMatrices.size(); ++i)
        {
            directional_light->get_light_matrices_ubo()->set_data(&lightSpaceMatrices[i], sizeof(glm::mat4x4),
                                                                  i * sizeof(glm::mat4x4));
        }

        // Setup fbo and shader.
        m_directional_light_shadows_shader->bind();
        glBindFramebuffer(GL_FRAMEBUFFER, directional_light->get_shadow_map_fbo());
        renderer::set_viewport_size(directional_light->get_shadow_map_dimensions());
        glClear(GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_FRONT);

        // Render geometry.
        render_geometry();

        // Reset state.
        glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_directional_light_shadows_shader->un_bind();

        // Reset
        renderer::set_viewport_size(renderer::get_viewport_size());
        renderer::clear_screen();
        renderer::set_state(renderer_state::depth, false);
    }

    void shadows_pass::render_geometry()
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
