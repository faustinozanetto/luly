#include "lypch.h"
#include "bloom_pass.h"

#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    bloom_pass::bloom_pass() : render_pass("bloom_pass")
    {
        bloom_pass::initialize();
    }

    bloom_pass::~bloom_pass()
    {
    }

    void bloom_pass::initialize()
    {
        // Setup pass frame buffer.
        glm::ivec2 viewport_size = renderer::get_viewport_size();

        m_samples_count = 4;
        m_mips.resize(m_samples_count);

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y);

        for (int i = 0; i < m_samples_count; i++)
        {
            bloom_pass_mip_data bloom_pass_mip_data;
            viewport_size *= 0.5f;
            viewport_size /= 2;
            bloom_pass_mip_data.size = viewport_size;

            texture_specification mip_texture_specification;
            mip_texture_specification.width = bloom_pass_mip_data.size.x;
            mip_texture_specification.height = bloom_pass_mip_data.size.y;
            mip_texture_specification.channels = 3;
            mip_texture_specification.mip_map_levels = 1;
            mip_texture_specification.internal_format = texture_internal_format::r11g11b10;
            mip_texture_specification.data = nullptr;

            bloom_pass_mip_data.texture = std::make_shared<texture_2d>(mip_texture_specification);

            m_mips[i] = bloom_pass_mip_data;
        }

        // Attach first bloom mip texture to rbo.
        m_fbo->attach_texture(m_mips[0].texture, GL_FRAMEBUFFER,
                              render_buffer_attachment_type::color, GL_TEXTURE_2D,
                              true, 0);
        m_fbo->initialize();

        // Load shaders
      m_down_sample_shader = shader_factory::create_shader_from_file(
            "assets/shaders/bloom/bloom_down_sample.rrs");
       m_up_sample_shader = shader_factory::create_shader_from_file(
            "assets/shaders/bloom/bloom_up_sample.rrs");
       m_composition_shader = shader_factory::create_shader_from_file(
            "assets/shaders/bloom/bloom_composition.rrs");
        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void bloom_pass::execute()
    {
        m_fbo->bind();
        renderer::clear_screen();
        m_screen_shader->bind();

        const render_pass_input& skybox_pass_input = m_inputs.at("skybox_pass_input");
        const render_pass_output& skybox_output = skybox_pass_input.render_pass->get_output("skybox_output");

        renderer::bind_texture(0, skybox_output.output);
        renderer::submit_mesh(m_screen_mesh);

        m_screen_shader->un_bind();
        m_fbo->un_bind();
    }

    void bloom_pass::set_outputs()
    {
        render_pass_output final_output;
        final_output.name = "final_output";
        final_output.output = m_fbo->get_attachment_id(0);
        add_output(final_output);
    }

    void bloom_pass::on_resize(const glm::ivec2& dimensions)
    {
        m_fbo->resize(dimensions);
    }
}
