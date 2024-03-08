#include "lypch.h"
#include "scene_renderer.h"

#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/skybox_component.h"
#include "application/application.h"
#include "renderer/renderer/debug/debug_renderer.h"
#include "renderer/renderer/pipeline/antialiasing_pass.h"
#include "renderer/renderer/pipeline/bloom_pass.h"
#include "renderer/renderer/pipeline/debanding_pass.h"
#include "renderer/renderer/pipeline/debug_pass.h"
#include "renderer/renderer/pipeline/environment_pass.h"
#include "renderer/renderer/pipeline/skybox_pass.h"
#include "renderer/renderer/pipeline/tonemapping_pass.h"
#include "renderer/renderer/pipeline/lighting/lighting_pass.h"

namespace luly::renderer
{
    scene_renderer_data scene_renderer::s_data = {};

    void scene_renderer::initialize()
    {
        LY_TRACE("Scene renderer initialization started...");
        create_pipeline_passes();
        create_camera_data();
        create_common_data();
        LY_TRACE("Scene renderer initialized successfully!");
    }

    void scene_renderer::begin_render(const std::shared_ptr<camera>& camera)
    {
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();
        if (!current_scene) return;

        /* Camera setup */
        s_data.camera = camera;
        update_camera_data();
        update_camera_buffer();

        get_render_pass<debug_pass>(render_pass_type::debug_pass)->reset_stats();
        debug_renderer::collect_debugables();

        for (const auto& [pass_name, pass] : s_data.passes)
        {
            pass->execute();
        }
    }

    void scene_renderer::end_render()
    {
    }

    void scene_renderer::set_outputs()
    {
        for (const auto& [pass_name, pass] : s_data.passes)
        {
            pass->set_outputs();
        }
    }

    void scene_renderer::set_environment_map(const std::shared_ptr<texture_2d>& environment_map)
    {
        const std::shared_ptr<environment_pass>& environment_render_pass = get_render_pass<environment_pass>(
            render_pass_type::environment_pass);
        environment_render_pass->set_environment_map(environment_map);
    }

    void scene_renderer::on_resize(const glm::ivec2& viewport_size)
    {
        for (const auto& [pass_name, pass] : s_data.passes)
        {
            pass->on_resize(viewport_size);
        }
    }

    void scene_renderer::create_pipeline_passes()
    {
        LY_TRACE("Started creating pipeling passes...");
        const auto& shadows_render_pass = std::make_shared<shadows_pass>();
        s_data.passes.insert({render_pass_type::shadow_pass, shadows_render_pass});

        const auto& geometry_render_pass = std::make_shared<geometry_pass>();
        s_data.passes.insert({render_pass_type::geometry_pass, geometry_render_pass});

        const auto& environment_render_pass = std::make_shared<environment_pass>();
        environment_render_pass->add_input({geometry_render_pass, "geometry_pass_input"});
        s_data.passes.insert({render_pass_type::environment_pass, environment_render_pass});

        const auto& ambient_occlusion_render_pass = std::make_shared<ambient_occlusion_pass>();
        ambient_occlusion_render_pass->add_input({geometry_render_pass, "geometry_pass_input"});
        s_data.passes.insert({render_pass_type::ambient_occlusion_pass, ambient_occlusion_render_pass});

        const auto& lighting_render_pass = std::make_shared<lighting_pass>();
        lighting_render_pass->add_input({geometry_render_pass, "geometry_pass_input"});
        lighting_render_pass->add_input({environment_render_pass, "environment_pass_input"});
        lighting_render_pass->add_input({ambient_occlusion_render_pass, "ambient_occlusion_pass_input"});
        lighting_render_pass->add_input({shadows_render_pass, "shadows_pass_input"});
        s_data.passes.insert({render_pass_type::lighting_pass, lighting_render_pass});

        const auto& skybox_render_pass = std::make_shared<skybox_pass>();
        skybox_render_pass->add_input({environment_render_pass, "environment_pass_input"});
        skybox_render_pass->add_input({lighting_render_pass, "lighting_pass_input"});
        s_data.passes.insert({render_pass_type::skybox_pass, skybox_render_pass});

        const auto& bloom_render_pass = std::make_shared<bloom_pass>();
        bloom_render_pass->add_input({skybox_render_pass, "skybox_pass_input"});
        s_data.passes.insert({render_pass_type::bloom_pass, bloom_render_pass});

        const auto& tonemapping_render_pass = std::make_shared<tonemapping_pass>();
        tonemapping_render_pass->add_input({bloom_render_pass, "bloom_pass_input"});
        s_data.passes.insert({render_pass_type::tonemapping_pass, tonemapping_render_pass});

        /*
        const auto& antialiasing_render_pass = std::make_shared<antialiasing_pass>();
        antialiasing_render_pass->add_input({tonemapping_render_pass, "tonemapping_pass_input"});
        s_data.passes.insert({render_pass_type::antialiasing_pass, antialiasing_render_pass});
*/
        const auto& debanding_render_pass = std::make_shared<debanding_pass>();
        debanding_render_pass->add_input({tonemapping_render_pass, "tonemapping_pass_input"});
        s_data.passes.insert({render_pass_type::debanding_pass, debanding_render_pass});

        const auto& debug_render_pass = std::make_shared<debug_pass>();
        debug_render_pass->add_input({debanding_render_pass, "debanding_pass_input"});
        s_data.passes.insert({render_pass_type::debug_pass, debug_render_pass});

        const auto& final_render_pass = std::make_shared<final_pass>();
        final_render_pass->add_input({debug_render_pass, "debug_pass_input"});
        s_data.passes.insert({render_pass_type::final_pass, final_render_pass});
        LY_TRACE("Pipeline passes created successfully!");
    }

    void scene_renderer::create_camera_data()
    {
        s_data.camera_ubo = std::make_shared<uniform_buffer_object>(sizeof(camera_data), CAMERA_UBO_LOCATION);
    }

    void scene_renderer::create_common_data()
    {
    }

    void scene_renderer::update_camera_data()
    {
        s_data.camera_data.view_matrix = s_data.camera->get_view_matrix();
        s_data.camera_data.projection_matrix = s_data.camera->get_projection_matrix();
        s_data.camera_data.view_projection_matrix = s_data.camera_data.
                                                           projection_matrix * s_data.camera_data.view_matrix;
        s_data.camera_data.position = glm::vec4(s_data.camera->get_position(), 1.0);
        s_data.camera_data.near_clip = s_data.camera->get_near_clip();
        s_data.camera_data.far_clip = s_data.camera->get_far_clip();
    }

    void scene_renderer::update_camera_buffer()
    {
        s_data.camera_ubo->set_data(&s_data.camera_data, sizeof(camera_data));
    }
}
