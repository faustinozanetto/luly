#include "pbr_mask_scene.h"

#include "scene_utils.h"
#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/models/model.h"
#include "renderer/models/model_factory.h"
#include "renderer/textures/texture_2d.h"
#include "renderer/textures/texture_factory.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

pbr_mask_scene::pbr_mask_scene() : scene("PBR Mask Scene")
{
    initialize();
}

pbr_mask_scene::~pbr_mask_scene()
{
}

void pbr_mask_scene::initialize()
{
    scene_utils::create_environment(this);
    create_mask();
}


void pbr_mask_scene::create_mask()
{
    const std::shared_ptr<luly::scene::scene_actor>& actor = create_actor("Plague Mask");
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

    /*
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
*/
    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});
    /*
    textures.insert({luly::renderer::material_texture_type::normal, normal});
    textures.insert({luly::renderer::material_texture_type::metallic, metallic});
    textures.insert({luly::renderer::material_texture_type::roughness, roughness});
    textures.insert({luly::renderer::material_texture_type::ambient_occlusion, ao});
*/
    const luly::renderer::material_specification& helmet_material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->
        with_textures(textures).build();

    const std::shared_ptr<luly::renderer::material>& plague_mask_material = std::make_shared<
        luly::renderer::material>(
        helmet_material_specification);
    /*
    plague_mask_material->set_texture_channel_mode(luly::renderer::material_texture_type::roughness,
                                                   luly::renderer::material_texture_channel_mode::green);
    plague_mask_material->set_texture_channel_mode(luly::renderer::material_texture_type::metallic,
                                                   luly::renderer::material_texture_channel_mode::blue);*/

    const std::shared_ptr<luly::assets::asset>& tv_material_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::material>(
        "plague-mask-material", luly::assets::asset_type::material, plague_mask_material);

    actor->add_component<luly::scene::material_component>(tv_material_asset->get_data<luly::renderer::material>());
}
