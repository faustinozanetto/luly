#include "lypch.h"
#include "lighting_pass.h"

#include "application/application.h"
#include "assets/asset_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/renderer/pipeline/shadows/shadows_pass.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/rendering/skybox_component.h"
#include "utils/PoissonGenerator.h"

namespace luly::renderer
{
    lighting_pass::lighting_pass() : render_pass("lighting_pass")
    {
        lighting_pass::initialize();
    }

    lighting_pass::~lighting_pass()
    {
    }

    void lighting_pass::initialize()
    {
        LY_PROFILE_FUNCTION;
        // Create pass frame buffer.
        const glm::ivec2& viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::rgba16f,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge, viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);
        m_fbo->initialize();

        m_lights_manager = std::make_shared<lights_manager>();

        generate_poisson_texture();

        // Load shader.
        m_lighting_shader = shader_factory::create_shader_from_file("assets/shaders/lighting_pass_shader.lsh");
        assets::asset_factory::create_asset("lighting_pass_shader-shader", assets::asset_type::shader,
                                            m_lighting_shader);

        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void lighting_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        // Set state
        renderer::set_state(renderer_state::depth, false);
        m_fbo->bind();
        renderer::clear_screen();
        m_lighting_shader->bind();

        m_lights_manager->update_lights();

        // Retrieve render pass inputs
        const render_pass_input& geometry_pass_input = m_inputs.at("geometry_pass_input");
        const render_pass_input& environment_pass_input = m_inputs.at("environment_pass_input");
        //  const render_pass_input& ambient_occlusion_pass_input = m_inputs.at("ambient_occlusion_pass_input");
        const render_pass_input& shadows_pass_input = m_inputs.at("shadows_pass_input");

        // Retrieve render pass inputs outputs.
        const render_pass_output& geometry_position_output = geometry_pass_input.render_pass->get_output(
            "position_output");
        const render_pass_output& geometry_albedo_output = geometry_pass_input.render_pass->get_output("albedo_output");
        const render_pass_output& geometry_normals_output = geometry_pass_input.render_pass->get_output(
            "normals_output");
        const render_pass_output& geometry_rough_metal_ao_output = geometry_pass_input.render_pass->get_output(
            "rough_metal_ao_output");
        const render_pass_output& geometry_emissive_output = geometry_pass_input.render_pass->get_output(
            "emissive_output");

        const render_pass_output& directional_shadow_map_output = shadows_pass_input.render_pass->get_output(
            "directional_shadow_map_output");

        // Blit depth from geometry pass to this fbo.
        int width = m_fbo->get_width();
        int height = m_fbo->get_height();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, geometry_pass_input.render_pass->get_fbo()->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->get_handle_id());
        renderer::blit_frame_buffer({0, 0}, {width, height}, {0, 0}, {width, height}, renderer_bit_mask::depth,
                                    texture_filtering::nearest);

        // Bind geometry pass outputs.
        renderer::bind_texture(0, geometry_position_output.output);
        renderer::bind_texture(1, geometry_albedo_output.output);
        renderer::bind_texture(2, geometry_normals_output.output);
        renderer::bind_texture(3, geometry_rough_metal_ao_output.output);
        renderer::bind_texture(4, geometry_emissive_output.output);
        // Bind environment pass outputs.
        renderer::bind_texture(5, environment_pass_input.render_pass->get_output("irradiance_output").output);
        renderer::bind_texture(6, environment_pass_input.render_pass->get_output("prefilter_output").output);
        renderer::bind_texture(7, environment_pass_input.render_pass->get_output("brdf_output").output);
        // Bind ssao output.
        //  renderer::bind_texture(8, ambient_occlusion_pass_input.render_pass->get_output("ssao_blur_output").output);
        // Bind shadow pass outputs.
        renderer::bind_texture(9, directional_shadow_map_output.output);
        renderer::bind_texture(10, m_poisson_distribution_texture->get_handle_id());

        std::vector<uint32_t> point_light_shadow_maps;
        const std::vector<scene::point_light_component>& point_lights = scene::scene_manager::get().get_current_scene()
            ->get_point_lights();
        point_light_shadow_maps.reserve(point_lights.size());
        for (const scene::point_light_component& point_light : point_lights)
        {
            point_light_shadow_maps.push_back(point_light.get_point_light()->get_shadow_cubemap());
        }
        glBindTextures(11, point_lights.size(), point_light_shadow_maps.data());

        // Update rest of uniforms
        upload_skybox_uniforms();
        const std::shared_ptr<shadows_pass>& shadows_render_pass = scene_renderer::get_render_pass<shadows_pass>(
            render_pass_type::shadow_pass);
        shadows_render_pass->bind_uniforms(m_lighting_shader);

        // Upload directional light space matrices.
        const std::vector<scene::directional_light_component>& directional_light_components =
            scene::scene_manager::get().get_current_scene()->get_directional_light();
        if (!directional_light_components.empty())
        {
            directional_light_components.front().get_directional_light()->
                                         upload_light_space_matrices(m_lighting_shader);
        }

        // Render screen quad mesh.
        renderer::submit_mesh(m_screen_mesh);

        m_lighting_shader->un_bind();
        m_fbo->un_bind();
    }

    void lighting_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output lighting_output;
        lighting_output.name = "lighting_output";
        lighting_output.output = m_fbo->get_attachment_id(0);
        add_output(lighting_output);
    }

    void lighting_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }

    void lighting_pass::generate_poisson_texture()
    {
        LY_PROFILE_FUNCTION;
        PoissonGenerator::DefaultPRNG prng;

        size_t attempts = 0;
        int samples = 64;
        int total_samples = samples * samples;
        std::vector<PoissonGenerator::Point> points = generatePoissonPoints(total_samples, prng, false, 40);
        while (points.size() < total_samples && ++attempts < 100)
        {
            points = generatePoissonPoints(total_samples, prng);
        }

        if (attempts == 100)
            LY_ASSERT_MSG(false, "Failed to generate Poisson distribution texture!")

        std::vector<glm::vec2> data(total_samples);
        for (auto i = 0; i < total_samples; i++)
        {
            const PoissonGenerator::Point& point = points[i];
            data[i] = {point.x, point.y};
        }

        texture_specification texture_specification = {samples, samples, 2, data.data()};
        m_poisson_distribution_texture = std::make_shared<texture_2d>(texture_specification);
    }

    void lighting_pass::upload_skybox_uniforms() const
    {
        LY_PROFILE_FUNCTION;
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        const auto& view = current_scene->get_registry()->view<scene::skybox_component>();
        for (const auto& [actor, skybox_component] : view.each())
        {
            m_lighting_shader->set_float("u_skybox_intensity", skybox_component.get_intensity());
            m_lighting_shader->set_vec_float4("u_skybox_tint", glm::vec4(skybox_component.get_tint(), 1.0));
        }
    }
}
