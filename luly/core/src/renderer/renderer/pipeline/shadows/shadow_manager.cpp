#include "lypch.h"
#include "shadow_manager.h"

#include "renderer/renderer/renderer.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/rendering/model_renderer_component.h"

namespace luly::renderer
{
    void shadow_manager::render_geometry(const std::shared_ptr<shader>& shader)
    {
        LY_PROFILE_FUNCTION;
        const std::shared_ptr<scene::scene>& current_scene = scene::scene_manager::get().get_current_scene();

        const auto& registry = current_scene->get_registry();
        const auto& view = registry->view<scene::transform_component, scene::model_renderer_component>();

        for (auto [actor, transform_component, model_renderer_component] : view.each())
        {
            const std::shared_ptr<math::transform>& transform = transform_component.get_transform();
            const std::shared_ptr<model>& model = model_renderer_component.get_model();

            if (!model_renderer_component.get_casts_shadows()) continue;

            shader->set_mat4("u_transform", transform->get_transform());

            renderer::submit_model(model);
        }
    }
}
