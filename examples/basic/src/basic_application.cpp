#include "basic_application.h"

#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "renderer/renderer/renderer.h"
#include "renderer/textures/texture_factory.h"
#include "scene/scene.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/lights/spot_light_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/rendering/skybox_component.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    luly::ui::engine_ui::initialize();

    setup_scene();

    luly::ui::engine_ui::set_render_target(
        luly::renderer::scene_renderer::get_data().final_pass->get_output("final_output").output);
}

basic_application::~basic_application()
{
    luly::ui::engine_ui::shutdown();
}

void basic_application::on_create()
{
}

void basic_application::on_update()
{
    application::on_update();

    const std::shared_ptr<luly::scene::scene>& current_scene = luly::scene::scene_manager::get().get_current_scene();
    if (!current_scene) return;

    luly::ui::engine_ui::begin_frame();

    current_scene->get_camera_manager()->get_perspective_camera_controller()->on_update(
        luly::app_time::get_delta_time());

    const std::shared_ptr<luly::renderer::perspective_camera>& camera = current_scene->get_camera_manager()->
        get_perspective_camera();

    luly::renderer::scene_renderer::begin_render(camera);
    luly::renderer::scene_renderer::end_render();

    luly::ui::engine_ui::on_update();
    luly::ui::engine_ui::end_frame();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
}

void basic_application::setup_scene()
{
    const std::shared_ptr<luly::scene::scene>& scene = std::make_shared<luly::scene::scene>("Test Scene");

    luly::scene::scene_manager::get().add_scene(scene);

    scene->get_camera_manager()->get_perspective_camera()->set_far_clip(750.0f);
    
    const std::shared_ptr<luly::scene::scene_actor>& dir_light_actor = scene->create_actor("Light Emitter");
    const std::shared_ptr<luly::renderer::directional_light>& directional_light = std::make_shared<
        luly::renderer::directional_light>(glm::vec3(0.85f), glm::vec3(-2.36f, -1.98f, 0.5f));
    directional_light->set_shadow_bias(0.001f);
    directional_light->set_z_multiplier(1.5f);
    dir_light_actor->add_component<luly::scene::directional_light_component>(directional_light);
    /*
    
    /*
    const std::shared_ptr<luly::renderer::mesh>& sphere_mesh = luly::renderer::mesh_factory::create_sphere_mesh(
        10, 10, 0.05f);
    float grid_gap = 0.2f;
    int rows = 1;
    int cols = 1;

    float startX = -(rows - 1) * grid_gap * 0.5f;
    float startY = -(cols - 1) * grid_gap * 0.5f;

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            float xPos = startX + row * grid_gap;
            float yPos = startY + col * grid_gap;

            // Calculate color based on row and col
            glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
            color.r += row / static_cast<float>(rows);
            color.g += col / static_cast<float>(cols);

            const std::shared_ptr<luly::scene::scene_actor>& point_light_actor = scene->create_actor(
                std::format("Point Light: [{},{}]", row, col));

            // Point light component with dynamic color
            point_light_actor->add_component<luly::scene::point_light_component>(
                std::make_shared<luly::renderer::point_light>(color));

            // Model renderer component
            point_light_actor->add_component<luly::scene::model_renderer_component>(
                luly::renderer::model_factory::create_model_from_meshes({
                    sphere_mesh
                }));

            // Transform component.
            const luly::scene::transform_component& transform_component =
                point_light_actor->get_component<luly::scene::transform_component>();

            transform_component.get_transform()->set_location(
                glm::vec3(xPos, yPos, 0.75f)
            );
            transform_component.get_transform()->set_scale({
                0.3f, 0.3f, 0.3f
            });
        }
    }
*/
    const std::shared_ptr<luly::scene::scene_actor>& skybox_actor = scene->create_actor("Skybox Actor");
    skybox_actor->add_component<luly::scene::skybox_component>(
        luly::renderer::texture_factory::create_environment_texture_from_file(
            "assets/hdris/kloofendal_43d_clear_puresky_4k.hdr"));

    /*
    const auto& spot_light_actor = scene->create_actor("Spot Light Emitter");
    spot_light_actor->add_component<luly::scene::spot_light_component>(
        std::make_shared<luly::renderer::spot_light>(glm::vec3(0.85f), glm::vec3(0, 0, 3), glm::vec3(0, 0, -1), 12.5f,
                                                     25.0f));
    spot_light_actor->get_component<luly::scene::transform_component>().get_transform()->set_location({0, 0, 2});
*/

    const std::shared_ptr<luly::scene::scene_actor>& actor = scene->create_actor("Sponza");
    actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({0.01f, 0.01f, 0.01f});

    // Loading model and creating asset.
    const std::shared_ptr<luly::renderer::model> sponza_model = luly::renderer::model_factory::create_model_from_file(
        "assets/models/sponza/sponza.obj");
    const std::shared_ptr<luly::assets::asset>& sponza_model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "sponza-model", luly::assets::asset_type::model, sponza_model);
    actor->add_component<luly::scene::model_renderer_component>(sponza_model_asset->get_data<luly::renderer::model>());

    /*
    const std::shared_ptr<luly::renderer::texture_2d>& albedo_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-albedo.png");
    const std::shared_ptr<luly::assets::asset>& albedo_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-albedo-texture", luly::assets::asset_type::texture, albedo_texture);
    luly::renderer::material_texture albedo = {albedo_texture, luly::renderer::material_texture_type::albedo, true};

    const std::shared_ptr<luly::renderer::texture_2d>& normal_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-normal.png");
    const std::shared_ptr<luly::assets::asset>& normal_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-normal-texture", luly::assets::asset_type::texture, normal_texture);
    luly::renderer::material_texture normal = {normal_texture, luly::renderer::material_texture_type::normal, true};

    const std::shared_ptr<luly::renderer::texture_2d>& metallic_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-metallic.png");
    const std::shared_ptr<luly::assets::asset>& metallic_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-metallic-texture", luly::assets::asset_type::texture, metallic_texture);
    luly::renderer::material_texture metallic = {
        metallic_texture, luly::renderer::material_texture_type::metallic, true
    };

    const std::shared_ptr<luly::renderer::texture_2d>& roughness_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-roughness.png");
    const std::shared_ptr<luly::assets::asset>& roughness_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-roughness-texture", luly::assets::asset_type::texture, roughness_texture);
    luly::renderer::material_texture roughness = {
        roughness_texture, luly::renderer::material_texture_type::roughness, true
    };

    const std::shared_ptr<luly::renderer::texture_2d>& ao_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/tv/tv-ao.png");
    const std::shared_ptr<luly::assets::asset>& ao_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "tv-ao-texture", luly::assets::asset_type::texture, ao_texture);
    luly::renderer::material_texture ao = {
        ao_texture, luly::renderer::material_texture_type::ambient_occlusion, true
    };

    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});
    textures.insert({luly::renderer::material_texture_type::ambient_occlusion, ao});

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->
        with_textures(textures).build();

    const std::shared_ptr<luly::renderer::material>& tv_material = std::make_shared<luly::renderer::material>(
        material_specification);

    const std::shared_ptr<luly::assets::asset>& tv_material_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::material>(
        "tv-material", luly::assets::asset_type::material, tv_material);

    m_actor->add_component<luly::scene::material_component>(tv_material_asset->get_data<luly::renderer::material>());

    // Create background actor
    /*
    const std::shared_ptr<luly::scene::scene_actor>& background_actor = scene->create_actor("Background Actor");
    const std::shared_ptr<luly::renderer::model> background_model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/floor.obj");
    const std::shared_ptr<luly::assets::asset>& background_model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "background-model", luly::assets::asset_type::model, background_model);
    luly::scene::model_renderer_component& background_model_renderer_component = background_actor->add_component<
        luly::scene::model_renderer_component>(
        background_model_asset->get_data<luly::renderer::model>());
    background_model_renderer_component.set_casts_shadows(false);
    background_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({5, 1, 5});
    background_actor->get_component<luly::scene::transform_component>().get_transform()->set_location({0, -3, 0});
    */

    luly::scene::scene_manager::get().switch_scene(scene);
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
