#include "lypch.h"
#include "scene_renderer.h"

#include "renderer/renderer/pipeline/geometry_pass.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/renderer/renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/skybox_component.h"
#include "application/application.h"

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

        s_data.shadows_pass->execute();

        s_data.geometry_pass->execute();

        // s_data.ambient_occlusion_pass->execute();

        s_data.lighting_pass->execute();
        s_data.skybox_pass->execute();
        s_data.bloom_pass->execute();
        s_data.tonemapping_pass->execute();
        s_data.debanding_pass->execute();
        s_data.final_pass->execute();
    }

    void scene_renderer::end_render()
    {
    }

    void scene_renderer::set_outputs()
    {
        s_data.shadows_pass->set_outputs();
        s_data.geometry_pass->set_outputs();
        //  s_data.ambient_occlusion_pass->set_outputs();
        s_data.lighting_pass->set_outputs();
        s_data.skybox_pass->set_outputs();
        s_data.bloom_pass->set_outputs();
        s_data.tonemapping_pass->set_outputs();
        s_data.debanding_pass->set_outputs();
        s_data.final_pass->set_outputs();
    }

    void scene_renderer::set_environment_map(const std::shared_ptr<texture_2d>& environment_map)
    {
        s_data.environment_pass->set_environment_map(environment_map);
    }

    void scene_renderer::on_resize(const glm::ivec2& viewport_size)
    {
        s_data.geometry_pass->on_resize(viewport_size);
        s_data.lighting_pass->on_resize(viewport_size);
        s_data.skybox_pass->on_resize(viewport_size);
        //s_data.ambient_occlusion_pass->on_resize(viewport_size);
        s_data.bloom_pass->on_resize(viewport_size);
        s_data.tonemapping_pass->on_resize(viewport_size);
        s_data.debanding_pass->on_resize(viewport_size);
        s_data.final_pass->on_resize(viewport_size);
    }

    scene_renderer_data& scene_renderer::get_data()
    {
        return s_data;
    }

    void scene_renderer::create_pipeline_passes()
    {
        LY_TRACE("Started creating pipeling passes...");
        s_data.geometry_pass = std::make_shared<geometry_pass>();

        s_data.shadows_pass = std::make_shared<shadows_pass>();

        s_data.environment_pass = std::make_shared<environment_pass>();
        s_data.environment_pass->add_input({s_data.geometry_pass, "geometry_pass_input"});

        // s_data.ambient_occlusion_pass = std::make_shared<ambient_occlusion_pass>();
        //s_data.ambient_occlusion_pass->add_input({s_data.geometry_pass, "geometry_pass_input"});

        s_data.lighting_pass = std::make_shared<lighting_pass>();
        s_data.lighting_pass->add_input({s_data.geometry_pass, "geometry_pass_input"});
        s_data.lighting_pass->add_input({s_data.environment_pass, "environment_pass_input"});
        // s_data.lighting_pass->add_input({s_data.ambient_occlusion_pass, "ambient_occlusion_pass_input"});
        s_data.lighting_pass->add_input({s_data.shadows_pass, "shadows_pass_input"});

        s_data.skybox_pass = std::make_shared<skybox_pass>();
        s_data.skybox_pass->add_input({s_data.environment_pass, "environment_pass_input"});
        s_data.skybox_pass->add_input({s_data.lighting_pass, "lighting_pass_input"});

        s_data.bloom_pass = std::make_shared<bloom_pass>();
        s_data.bloom_pass->add_input({s_data.skybox_pass, "skybox_pass_input"});

        s_data.tonemapping_pass = std::make_shared<tonemapping_pass>();
        s_data.tonemapping_pass->add_input({s_data.bloom_pass, "bloom_pass_input"});

        s_data.debanding_pass = std::make_shared<debanding_pass>();
        s_data.debanding_pass->add_input({s_data.tonemapping_pass, "tonemapping_pass_input"});

        s_data.final_pass = std::make_shared<final_pass>();
        s_data.final_pass->add_input({s_data.debanding_pass, "debanding_pass_input"});
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
