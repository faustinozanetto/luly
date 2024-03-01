#include "basic_application.h"

#include <application/entry_point.h>

#include "engine_ui.h"
#include "imgui.h"
#include "renderer/models/model_factory.h"
#include "renderer/textures/texture_factory.h"
#include "scene/scene.h"

#include <events/event_dispatcher.h>
#include <time/app_time.h>

#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "renderer/materials/material.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/meshes/mesh_factory.h"
#include "renderer/renderer/pipeline/final_pass.h"
#include "renderer/scene/scene_renderer.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/lights/directional_light_component.h"
#include "scene/actor/components/lights/point_light_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"
#include "scene/actor/components/rendering/skybox_component.h"

basic_application::basic_application(const luly::renderer::window_specification& window_specification) : application(
    window_specification)
{
    m_engine_ui = std::make_shared<luly::ui::engine_ui>();
    setup_scene();

    m_engine_ui->set_render_target(
        luly::renderer::scene_renderer::get_render_pass<luly::renderer::final_pass>(
            luly::renderer::render_pass_type::final_pass)->get_output("final_output").output);
}

basic_application::~basic_application()
{
}

void basic_application::on_create()
{
}

void basic_application::on_update()
{
    application::on_update();

    const std::shared_ptr<luly::scene::scene>& current_scene = luly::scene::scene_manager::get().get_current_scene();
    if (!current_scene) return;

    m_engine_ui->begin_frame();
    /*
        const std::shared_ptr<luly::renderer::directional_light>& directional_light = current_scene->
            get_directional_light();
    
        glm::vec3 direction = directional_light->get_direction();
        direction.z = sin(2 * glm::pi<float>() * 0.05f * luly::app_time::get_time());
        directional_light->set_direction(direction);*/

    const std::shared_ptr<luly::renderer::perspective_camera>& camera = current_scene->get_camera_manager()->
        get_perspective_camera();

    luly::renderer::scene_renderer::begin_render(camera);
    luly::renderer::scene_renderer::end_render();

    m_engine_ui->on_update();
    m_engine_ui->end_frame();
}

void basic_application::on_handle_event(luly::events::base_event& event)
{
    m_engine_ui->on_event(event);
}

void basic_application::setup_scene()
{
    const std::shared_ptr<luly::scene::scene>& scene = std::make_shared<luly::scene::scene>("Test Scene");

    luly::scene::scene_manager::get().add_scene(scene);

    scene->get_camera_manager()->get_perspective_camera()->set_far_clip(250.0f);

    const std::shared_ptr<luly::scene::scene_actor>& dir_light_actor = scene->create_actor("Sun Light");
    const std::shared_ptr<luly::renderer::directional_light>& directional_light = std::make_shared<
        luly::renderer::directional_light>();
    directional_light->set_direction(86.0f, 8.0f);
    dir_light_actor->add_component<luly::scene::directional_light_component>(directional_light);

    // Skybox
    const std::shared_ptr<luly::scene::scene_actor>& skybox_actor = scene->create_actor("Skybox");
    const std::shared_ptr<luly::renderer::texture_2d>& environment_texture =
        luly::renderer::texture_factory::create_environment_texture_from_file(
            "assets/hdris/blue_photo_studio_4k.hdr");
    luly::scene::skybox_component& skybox_component = skybox_actor->add_component<luly::scene::skybox_component>(
        environment_texture
    );
    skybox_component.set_intensity(0.5f);

    // Create emissive cube
    /*
    const std::shared_ptr<luly::scene::scene_actor>& emissive_cube_actor = scene->create_actor("Emissive Cube");
    emissive_cube_actor->add_component<luly::scene::model_renderer_component>(
        luly::renderer::model_factory::create_model_from_meshes({
            luly::renderer::mesh_factory::create_cube_mesh()
        }));

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_emissive({0.25f, 0.35f, 0.65f}).
                                                                            with_emissive_strength(5.0f).build();
    const std::shared_ptr<luly::renderer::material>& emissive_material = std::make_shared<luly::renderer::material>(
        material_specification);
    emissive_cube_actor->add_component<luly::scene::material_component>(emissive_material);
*/
    /*
        const std::shared_ptr<luly::scene::scene_actor>& sponza_actor = scene->create_actor("Bistro");
        sponza_actor->add_component<luly::scene::model_renderer_component>(
            luly::renderer::model_factory::create_model_from_file("assets/models/Bistro/bistro.gltf"));
        //sponza_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({0.01f,0.01f,0.01f});
    
        // Create emissive cube
        /*
        const std::shared_ptr<luly::scene::scene_actor>& emissive_cube_actor = scene->create_actor("Emissive Cube");
        emissive_cube_actor->add_component<luly::scene::model_renderer_component>(
            luly::renderer::model_factory::create_model_from_meshes({
                luly::renderer::mesh_factory::create_cube_mesh()
            }));
    
        const luly::renderer::material_specification& material_specification =
            std::make_shared<luly::renderer::material_specification_builder>()->with_emissive({0.25f, 0.35f, 0.65f}).
                                                                                with_emissive_strength(5.0f).build();
        const std::shared_ptr<luly::renderer::material>& emissive_material = std::make_shared<luly::renderer::material>(
            material_specification);
        emissive_cube_actor->add_component<luly::scene::material_component>(emissive_material);
    */
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
        }*/

    const std::shared_ptr<luly::scene::scene_actor>& actor = scene->create_actor("Plague Mask");
    const luly::scene::transform_component& actor_transform = actor->get_component<luly::scene::transform_component>();
    actor_transform.get_transform()->set_location({-0.15f, 1.1f, -0.5f});
    actor_transform.get_transform()->set_scale({0.85f, 0.85f, 0.85f});

    const std::shared_ptr<luly::renderer::model> plague_mask_model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/PlagueMask/PlagueMask.obj");
    const std::shared_ptr<luly::assets::asset>& plague_mask_model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "plague-mask-model", luly::assets::asset_type::model, plague_mask_model);
    actor->add_component<luly::scene::model_renderer_component>(
        plague_mask_model_asset->get_data<luly::renderer::model>());

    const std::shared_ptr<luly::renderer::texture_2d>& albedo_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Base_Color.png");
    const std::shared_ptr<luly::assets::asset>& albedo_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-albedo-texture", luly::assets::asset_type::texture, albedo_texture);
    luly::renderer::material_texture albedo = {albedo_texture, luly::renderer::material_texture_type::albedo};

    const std::shared_ptr<luly::renderer::texture_2d>& normal_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Normal_DirectX.png");
    const std::shared_ptr<luly::assets::asset>& normal_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-normal-texture", luly::assets::asset_type::texture, normal_texture);
    luly::renderer::material_texture normal = {normal_texture, luly::renderer::material_texture_type::normal};

    const std::shared_ptr<luly::renderer::texture_2d>& metallic_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Metallic.png");
    const std::shared_ptr<luly::assets::asset>& metallic_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-metallic-texture", luly::assets::asset_type::texture, metallic_texture);
    luly::renderer::material_texture metallic = {
        metallic_texture, luly::renderer::material_texture_type::metallic
    };

    const std::shared_ptr<luly::renderer::texture_2d>& roughness_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Roughness.png");
    const std::shared_ptr<luly::assets::asset>& roughness_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-roughness-texture", luly::assets::asset_type::texture, roughness_texture);
    luly::renderer::material_texture roughness = {
        roughness_texture, luly::renderer::material_texture_type::roughness
    };

    const std::shared_ptr<luly::renderer::texture_2d>& ao_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/textures/PlagueMask/PlagueMask_Mat_Mixed_AO.png");
    const std::shared_ptr<luly::assets::asset>& ao_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "plague-mask-ao-texture", luly::assets::asset_type::texture, ao_texture);
    luly::renderer::material_texture ao = {
        ao_texture, luly::renderer::material_texture_type::ambient_occlusion
    };

    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});
    textures.insert({luly::renderer::material_texture_type::ambient_occlusion, ao});

    const luly::renderer::material_specification& helmet_material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->
        with_textures(textures).build();

    const std::shared_ptr<luly::renderer::material>& plague_mask_material = std::make_shared<
        luly::renderer::material>(
        helmet_material_specification);
    plague_mask_material->set_texture_channel_mode(luly::renderer::material_texture_type::roughness,
                                                   luly::renderer::material_texture_channel_mode::green);
    plague_mask_material->set_texture_channel_mode(luly::renderer::material_texture_type::metallic,
                                                   luly::renderer::material_texture_channel_mode::blue);

    const std::shared_ptr<luly::assets::asset>& tv_material_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::material>(
        "plague-mask-material", luly::assets::asset_type::material, plague_mask_material);

    actor->add_component<luly::scene::material_component>(tv_material_asset->get_data<luly::renderer::material>());

    // Create background actor
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
    background_actor->get_component<luly::scene::transform_component>().get_transform()->set_scale({10, 1, 10});
    background_actor->get_component<luly::scene::transform_component>().get_transform()->set_location({0, 0, 0});

    luly::scene::scene_manager::get().switch_scene(scene);
}

luly::core::application* luly::core::create_application()
{
    const renderer::window_specification window_specification = {
        "Basic Application", 1920, 1080
    };
    return new basic_application(window_specification);
}
