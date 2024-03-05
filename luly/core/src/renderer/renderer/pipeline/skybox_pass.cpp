#include "lypch.h"
#include "skybox_pass.h"

#include "geometry_pass.h"
#include "assets/asset_factory.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/shaders/shader_factory.h"
#include "scene/scene_manager.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/rendering/skybox_component.h"

namespace luly::renderer
{
    skybox_pass::skybox_pass(): render_pass("skybox_pass")
    {
        skybox_pass::initialize();
    }

    skybox_pass::~skybox_pass()
    {
    }

    void skybox_pass::initialize()
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

        frame_buffer_attachment depth_attachment = {
            texture_internal_format::depth_component32,
            texture_filtering::linear,
            texture_wrapping::clamp_to_edge, viewport_size
        };

        m_fbo = std::make_shared<frame_buffer>(
            viewport_size.x, viewport_size.y, attachments, depth_attachment);
        m_fbo->initialize();

        // Load shaders.
        m_skybox_shader = shader_factory::create_shader_from_file("assets/shaders/skybox/skybox.lsh");
        assets::asset_factory::create_asset("skybox-shader", assets::asset_type::shader, m_skybox_shader);

        if (!assets::assets_manager::get().asset_already_registered("screen-shader"))
        {
            m_screen_shader = shader_factory::create_shader_from_file("assets/shaders/screen.lsh");
            assets::asset_factory::create_asset("screen-shader", assets::asset_type::shader, m_screen_shader);
        }
        else
        {
            m_screen_shader = assets::assets_manager::get().get_asset<assets::asset, assets::asset_type::shader>(
                "screen-shader")->get_data<shader>();
        }
        // Load meshes.
        m_cube_mesh = mesh_factory::create_cube_mesh();
        m_screen_mesh = mesh_factory::create_screen_quad_mesh();
    }

    void skybox_pass::execute()
    {
        LY_PROFILE_FUNCTION;
        const render_pass_input& lighting_pass_input = m_inputs.at("lighting_pass_input");
        const render_pass_input& environment_pass_input = m_inputs.at("environment_pass_input");
        const render_pass_output& environment_cubemap_texture = environment_pass_input.render_pass->get_output(
            "environment_cubemap_output");

        m_fbo->bind();

        renderer::set_state(renderer_state::depth, true);
        renderer::set_depth_func(renderer_depth_func::less_or_equal);

        int width = m_fbo->get_width();
        int height = m_fbo->get_height();

        // Copy depth & color buffer from lighting pass to this fbo.
        glBindFramebuffer(GL_READ_FRAMEBUFFER, lighting_pass_input.render_pass->get_fbo()->get_handle_id());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo->get_handle_id());
        renderer::blit_frame_buffer({0, 0}, {width, height}, {0, 0}, {width, height},
                                    renderer_bit_mask::color | renderer_bit_mask::depth, texture_filtering::nearest);

        const scene::skybox_component* skybox_component = scene::scene_manager::get().get_current_scene()->get_skybox();

        // Render skybox cube using the environment cubemap texture.
        m_skybox_shader->bind();
        m_skybox_shader->set_float("u_lod_level",
                                   skybox_component->get_lod_level());
        renderer::bind_texture(0, environment_cubemap_texture.output);
        renderer::submit_mesh(m_cube_mesh);
        m_skybox_shader->un_bind();

        renderer::set_depth_func(renderer_depth_func::less);
        m_fbo->un_bind();
    }

    void skybox_pass::set_outputs()
    {
        LY_PROFILE_FUNCTION;
        render_pass_output skybox_output;
        skybox_output.name = "skybox_output";
        skybox_output.output = m_fbo->get_attachment_id(0);
        add_output(skybox_output);
    }

    void skybox_pass::on_resize(const glm::ivec2& dimensions)
    {
        LY_PROFILE_FUNCTION;
        m_fbo->resize(dimensions);
    }
}
