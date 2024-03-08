#include "animations_scene.h"

#include "scene_utils.h"
#include "assets/asset.h"
#include "assets/asset_factory.h"
#include "renderer/animations/animation_clip.h"
#include "renderer/animations/animation_controller.h"
#include "renderer/materials/material_specification_builder.h"
#include "renderer/models/model.h"
#include "renderer/models/model_factory.h"
#include "renderer/textures/texture_2d.h"
#include "renderer/textures/texture_factory.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/animations/animation_controller_component.h"
#include "scene/actor/components/rendering/material_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

animations_scene::animations_scene() : scene("Animations Scene")
{
    initialize();
}

animations_scene::~animations_scene()
{
}

void animations_scene::on_update(float delta_time)
{
    scene::on_update(delta_time);

    const auto& view = get_registry()->view<luly::scene::animation_controller_component>();
    for (const auto [actor, animation_controller_component] : view.each())
    {
        animation_controller_component.get_animation_controller()->on_update(delta_time);
    }
}

void animations_scene::initialize()
{
    scene_utils::create_environment(this);
    create_animated_model();
    scene_utils::create_point_light(this, {2, 4, 2}, {0.2f, 0.2f, 0.2f});
}

void animations_scene::create_animated_model()
{
    const std::shared_ptr<luly::scene::scene_actor>& actor = create_actor("Animated Model");

    // 1 . Create model.
    const std::shared_ptr<luly::renderer::model>& model =
        luly::renderer::model_factory::create_model_from_file(
            "assets/models/TomCat/TomCatDanceSamba.fbx");
    const std::shared_ptr<luly::assets::asset>& model_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::model>(
        "animated-model", luly::assets::asset_type::model, model);
    actor->add_component<luly::scene::model_renderer_component>(
        model_asset->get_data<luly::renderer::model>());
    actor->get_component<luly::scene::transform_component>().get_transform()->set_scale(glm::vec3(0.2f));

    // 2. Create material
    const std::shared_ptr<luly::renderer::texture_2d>& albedo_texture =
        luly::renderer::texture_factory::create_texture_from_file(
            "assets/models/TomCat/TomCatAlbedo.png");
    const std::shared_ptr<luly::assets::asset>& albedo_texture_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::texture_2d>(
        "flamingo-albedo-texture", luly::assets::asset_type::texture, albedo_texture);
    luly::renderer::material_texture albedo = {albedo_texture, luly::renderer::material_texture_type::albedo};


    std::map<luly::renderer::material_texture_type, luly::renderer::material_texture> textures;
    textures.insert({luly::renderer::material_texture_type::albedo, albedo});

    const luly::renderer::material_specification& material_specification =
        std::make_shared<luly::renderer::material_specification_builder>()->with_textures(textures).build();
    const std::shared_ptr<luly::renderer::material>& material = std::make_shared<
        luly::renderer::material>(material_specification);

    const std::shared_ptr<luly::assets::asset>& material_asset = luly::assets::asset_factory::create_asset<
        luly::renderer::material>(
        "animated-model-material", luly::assets::asset_type::material, material);
    actor->add_component<luly::scene::material_component>(material_asset->get_data<luly::renderer::material>());

    // 3. Load animation
    const std::shared_ptr<luly::renderer::animation_clip>& animation_clip = std::make_shared<
        luly::renderer::animation_clip>("assets/models/TomCat/TomCatDanceSamba.fbx", model);

    const std::shared_ptr<luly::renderer::animation_controller>& animation_controller = std::make_shared<
        luly::renderer::animation_controller>(animation_clip);
    actor->add_component<luly::scene::animation_controller_component>(animation_controller);
}
