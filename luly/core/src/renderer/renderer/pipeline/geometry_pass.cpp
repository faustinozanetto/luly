#include "lypch.h"
#include "geometry_pass.h"

#include "application/application.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

namespace luly::renderer
{
    geometry_pass::geometry_pass() : render_pass("geometry_pass")
    {
        geometry_pass::initialize();
    }

    geometry_pass::~geometry_pass()
    {
    }

    void geometry_pass::initialize()
    {
        LY_PROFILE_FUNCTION;
        // Setup pass frame buffer.
        const glm::ivec2& viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            // Position
            {
                texture_internal_format::rgb16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
            // View Space Position
            {
                texture_internal_format::rgb16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
            // Albedo
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
            // Normal
            {
                texture_internal_format::rgb16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
            // Roughness-Metallic-AO
            {
                texture_internal_format::rgb16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
            //  Emissive
            {
                texture_internal_format::rgb16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
            // Actor ID
            {
                texture_internal_format::r8,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge,
                viewport_size
            },
        };

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge,
            viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);
        m_fbo->initialize();

        // Load shader.
        m_geometry_shader = shader_factory::create_shader_from_file("assets/shaders/geometry_pass_shader.lsh");
    }

    void geometry_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        m_fbo->bind();
        renderer::set_state(renderer_state::depth, true);
        static int actor_id_clear = -1;
        m_geometry_shader->bind();
        renderer::set_clear_color({0.1f, 0.1f, 0.1f, 1});
        renderer::clear_screen();
        m_fbo->clear_attachment(GEOMETRY_PASS_ACTOR_ID_ATTACHMENT_IDX, GL_RED_INTEGER, GL_INT, &actor_id_clear);

        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        const auto& registry = current_scene->get_registry();
        const auto& view = registry->view<scene::transform_component>();

        for (auto [actor, transform_component] : view.each())
        {
            const std::shared_ptr<math::transform>& transform = transform_component.get_transform();

            // Check if has model_renderer_component
            if (!registry->any_of<scene::model_renderer_component>(actor)) continue;

            const scene::model_renderer_component& model_renderer_component = registry->get<
                scene::model_renderer_component>(actor);
            const std::shared_ptr<model>& model = model_renderer_component.get_model();

            glm::mat3 normal_matrix = transpose(inverse(glm::mat3(transform->get_transform())));
            m_geometry_shader->set_mat3("u_normal_matrix", normal_matrix);
            m_geometry_shader->set_mat4("u_transform_matrix", transform->get_transform());
            m_geometry_shader->set_int("u_actor_id", static_cast<int>(actor));

            // Check if has material_component
            if (registry->any_of<scene::material_component>(actor))
            {
                scene::material_component& material_component = registry->get<scene::material_component>(actor);
                const std::shared_ptr<material>& material = material_component.get_material();

                material->bind(m_geometry_shader);
                renderer::submit_model(model);
            }

            // Check if model has no materials embedded.
            else if (model->get_materials().empty())
            {
                material::bind_default(m_geometry_shader);
                renderer::submit_model(model);
            }
            else
            {
                // Render model with embedded materials for the meshes.
                for (const std::shared_ptr<mesh>& model_mesh : model->get_meshes())
                {
                    const std::shared_ptr<material>& mesh_material = model->get_materials().at(model_mesh->get_name());
                    mesh_material->bind(m_geometry_shader);
                    renderer::submit_mesh(model_mesh);
                }
            }
        }

        /*
        // If model has no embedded materials render as usual.
        if (model->get_materials().empty() && has_material_component)
        {
            continue;
        }
        
        for (const std::shared_ptr<mesh>&model_mesh : model->get_meshes())
        {
            // Check if mesh material index is in range.
            const int material_index = model_mesh->get_material_index();
            if (material_index <= model->get_materials().size())
            {
                model->get_materials().at(material_index)->bind(m_geometry_shader);
            }
            renderer::submit_mesh(model_mesh);
        }*/

        m_geometry_shader->un_bind();
        m_fbo->un_bind();

        renderer::set_state(renderer_state::depth, false);
    }

    void geometry_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output position_output;
        position_output.name = "position_output";
        position_output.output = m_fbo->get_attachment_id(0);
        add_output(position_output);

        render_pass_output view_space_position_output;
        view_space_position_output.name = "view_space_position_output";
        view_space_position_output.output = m_fbo->get_attachment_id(1);
        add_output(view_space_position_output);

        render_pass_output albedo_output;
        albedo_output.name = "albedo_output";
        albedo_output.output = m_fbo->get_attachment_id(2);
        add_output(albedo_output);

        render_pass_output normals_output;
        normals_output.name = "normals_output";
        normals_output.output = m_fbo->get_attachment_id(3);
        add_output(normals_output);

        render_pass_output rough_metal_ao_output;
        rough_metal_ao_output.name = "rough_metal_ao_output";
        rough_metal_ao_output.output = m_fbo->get_attachment_id(4);
        add_output(rough_metal_ao_output);

        render_pass_output emissive_output;
        emissive_output.name = "emissive_output";
        emissive_output.output = m_fbo->get_attachment_id(5);
        add_output(emissive_output);

        render_pass_output actor_id_output;
        actor_id_output.name = "actor_id_output";
        actor_id_output.output = m_fbo->get_attachment_id(6);
        add_output(actor_id_output);

        render_pass_output depth_output;
        depth_output.name = "depth_output";
        depth_output.output = m_fbo->get_depth_attachment();
        add_output(depth_output);
    }

    void geometry_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }
}
