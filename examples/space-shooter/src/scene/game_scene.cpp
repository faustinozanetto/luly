#include "game_scene.h"

#include "renderer/meshes/mesh_factory.h"
#include "renderer/models/model_factory.h"
#include "scene/actor/scene_actor.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

game_scene::game_scene() : scene("Game Scene")
{
	initialize();
}

void game_scene::initialize()
{
	create_background_actor();
}

void game_scene::create_background_actor()
{
	const std::shared_ptr<luly::scene::scene_actor>& background_actor = create_actor("Background");
	const std::shared_ptr<luly::renderer::model>& background_model = luly::renderer::model_factory::create_model_from_meshes({ luly::renderer::mesh_factory::create_screen_quad_mesh() });
	background_actor->add_component<luly::scene::model_renderer_component>(background_model);
}
