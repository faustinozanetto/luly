#include "lypch.h"
#include "tonemapping_pass.h"

#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"

namespace luly::renderer
{
    tonemapping_pass::tonemapping_pass() : render_pass("tonemapping_pass")
    {
        tonemapping_pass::initialize();
    }

    tonemapping_pass::~tonemapping_pass()
    {
    }

    void tonemapping_pass::initialize()
    {
        LY_PROFILE_FUNCTION;
        // Setup pass frame buffer.
        const glm::ivec2& viewport_size = renderer::get_viewport_size();

        std::vector<frame_buffer_attachment> attachments = {
            {
                texture_internal_format::r11g11b10,
                texture_filtering::linear,
                texture_wrapping::clamp_to_edge, viewport_size
            },
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments);
        m_fbo->initialize();

        // Create shader.
        m_tonemapping_shader = shader_factory::create_shader_from_file("assets/shaders/tonemapping.lsh");

        // Create screen quad
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();

        m_gamma = 2.2f;
        m_exposure = 1.0f;
        m_type = tonemapping_type::linear;
    }

    void tonemapping_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        m_fbo->bind();
        renderer::clear_screen();
        m_tonemapping_shader->bind();

        const render_pass_input& bloom_pass_input = m_inputs.at("bloom_pass_input");
        const render_pass_output& bloom_output = bloom_pass_input.render_pass->get_output("bloom_output");

        // Upload uniforms
        m_tonemapping_shader->set_float("u_exposure", m_exposure);
        m_tonemapping_shader->set_float("u_gamma", m_gamma);
        m_tonemapping_shader->set_int("u_type", static_cast<int>(m_type));

        renderer::bind_texture(0, bloom_output.output);
        renderer::submit_mesh(m_screen_mesh);

        m_tonemapping_shader->un_bind();
        m_fbo->un_bind();
    }

    void tonemapping_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output tonemapping_output;
        tonemapping_output.name = "tonemapping_output";
        tonemapping_output.output = m_fbo->get_attachment_id(0);
        add_output(tonemapping_output);
    }

    void tonemapping_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }

    const char* tonemapping_pass::get_tonemapping_type_to_string(tonemapping_type type)
    {
        LY_PROFILE_FUNCTION;
        switch (type)
        {
        case tonemapping_type::linear:
            return "linear";
        case tonemapping_type::reinhard:
            return "reinhard";
        case tonemapping_type::aces:
            return "aces";
        case tonemapping_type::filmic:
            return "filmic";
        case tonemapping_type::hable:
            return "hable";
        case tonemapping_type::luma:
            return "luma";
        }
        LY_ASSERT_MSG(false, "Unknown tonemapping type!")
        return nullptr;
    }
}
