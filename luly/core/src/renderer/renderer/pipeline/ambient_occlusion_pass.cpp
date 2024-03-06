#include "lypch.h"
#include "ambient_occlusion_pass.h"

#include <random>

#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "renderer/textures/texture_factory.h"

namespace luly::renderer
{
    ambient_occlusion_pass::ambient_occlusion_pass() : render_pass("ambient_occlusion_pass")
    {
        ambient_occlusion_pass::initialize();
    }

    ambient_occlusion_pass::~ambient_occlusion_pass()
    {
    }

    void ambient_occlusion_pass::initialize()
    {
        LY_PROFILE_FUNCTION;
        // Create pass frame buffer.
        const glm::ivec2& viewport_size = renderer::get_viewport_size();
        std::vector<frame_buffer_attachment> fbo_attachments = {
            {
                texture_internal_format::r16f,
                texture_filtering::nearest,
                texture_wrapping::none, viewport_size
            },
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, fbo_attachments);
        m_fbo->initialize();

        // Create blur frame buffer.
        std::vector<frame_buffer_attachment> blur_fbo_attachments = {
            {
                texture_internal_format::r16f,
                texture_filtering::nearest,
                texture_wrapping::none, viewport_size
            },
        };

        m_blur_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, blur_fbo_attachments);
        m_blur_fbo->initialize();

        // Create shaders.
        m_ssao_shader = shader_factory::create_shader_from_file("assets/shaders/ambient_occlusion/ssao.lsh");
        assets::asset_factory::create_asset("ssao-shader", assets::asset_type::shader, m_ssao_shader);

        m_blur_shader = shader_factory::create_shader_from_file("assets/shaders/ambient_occlusion/blur.lsh");
        assets::asset_factory::create_asset("ssao-blur-shader", assets::asset_type::shader, m_blur_shader);

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();

        // Setup parameters.
        m_ssao_radius = 0.5f;
        m_ssao_bias = 0.025f;
        m_ssao_noise_size = 4.0f;
        m_ssao_kernel_size = 64;
        m_ssao_noise_sample_size = 16;

        // Setup ssao kernel
        std::uniform_real_distribution<float> random_floats(0.0, 1.0);
        std::default_random_engine generator;

        for (int i = 0; i < m_ssao_kernel_size; i++)
        {
            glm::vec3 sample(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0,
                             random_floats(generator));

            sample = normalize(sample);
            sample *= random_floats(generator);

            float scale = static_cast<float>(i) / static_cast<float>(m_ssao_kernel_size);
            scale = std::lerp(0.1f, 1.0f, scale * scale);
            sample *= scale;

            m_ssao_kernel.push_back(sample);
        }

        // Setup ssao noise texture.
        for (int i = 0; i < m_ssao_noise_sample_size; i++)
        {
            glm::vec3 noise(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0);
            m_ssao_noise.push_back(noise);
        }

        m_ssao_noise_texture = std::make_shared<texture_2d>(texture_specification({4, 4, 3, m_ssao_noise.data()}));
        m_ssao_noise_texture->set_filtering(texture_filtering_type::filter_min,
                                            texture_filtering::nearest);
        m_ssao_noise_texture->set_filtering(texture_filtering_type::filter_mag,
                                            texture_filtering::nearest);
        m_ssao_noise_texture->set_wrapping(texture_wrapping_type::wrap_s,
                                           texture_wrapping::repeat);
        m_ssao_noise_texture->set_wrapping(texture_wrapping_type::wrap_t,
                                           texture_wrapping::repeat);
    }

    void ambient_occlusion_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        m_fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        m_ssao_shader->bind();
        m_ssao_shader->set_float("u_ssao_bias", m_ssao_bias);
        m_ssao_shader->set_float("u_ssao_radius", m_ssao_radius);

        const auto noise_scale = glm::vec2(renderer::get_viewport_size().x / m_ssao_noise_size,
                                           renderer::get_viewport_size().y / m_ssao_noise_size);
        m_ssao_shader->set_vec_float2("u_ssao_noise_size", noise_scale);

        m_ssao_shader->set_int("u_ssao_kernel_size", m_ssao_kernel_size);
        for (int i = 0; i < m_ssao_kernel_size; i++)
        {
            m_ssao_shader->set_vec_float3("u_ssao_kernel[" + std::to_string(i) + "]", m_ssao_kernel[i]);
        }

        const render_pass_input& geometry_pass_input = m_inputs.at("geometry_pass_input");
        const render_pass_output& geometry_view_space_position_output = geometry_pass_input.render_pass->get_output(
            "view_space_position_output");
        const render_pass_output& geometry_normal_output = geometry_pass_input.render_pass->
                                                                               get_output("normals_output");

        renderer::bind_texture(0, geometry_view_space_position_output.output); // Geometry View Space Pass Position.
        renderer::bind_texture(1, geometry_normal_output.output); // Geometry Pass Normal
        renderer::bind_texture(2, m_ssao_noise_texture->get_handle_id()); // SSAO Noise
        renderer::submit_mesh(m_screen_mesh);

        m_ssao_shader->un_bind();
        m_fbo->un_bind();

        // Perform blur pass.
        m_blur_fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);
        m_blur_shader->bind();

        renderer::bind_texture(0, m_fbo->get_attachment_id(0));
        renderer::submit_mesh(m_screen_mesh);
        m_blur_shader->un_bind();
        m_blur_fbo->un_bind();
    }

    void ambient_occlusion_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output ssao_output;
        ssao_output.name = "ssao_output";
        ssao_output.output = m_fbo->get_attachment_id(0);
        add_output(ssao_output);

        render_pass_output ssao_blur_output;
        ssao_blur_output.name = "ssao_blur_output";
        ssao_blur_output.output = m_blur_fbo->get_attachment_id(0);
        add_output(ssao_blur_output);

        render_pass_output ssao_noise_output;
        ssao_noise_output.name = "ssao_noise_output";
        ssao_noise_output.output = m_ssao_noise_texture->get_handle_id();
        add_output(ssao_noise_output);
    }

    void ambient_occlusion_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
        m_blur_fbo->resize(dimensions);
    }
}
