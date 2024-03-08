#include "lypch.h"
#include "shadow_manager.h"

#include "renderer/renderer/renderer.h"
#include "scene/scene_manager.h"
#include "scene/actor/components/transform_component.h"
#include "scene/actor/components/animations/animation_controller_component.h"
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

            // Check if has animation_controller_component, if so, we need to upload final bone matrices
            const bool has_animation_controller = registry->any_of<scene::animation_controller_component>(actor);
            if (has_animation_controller)
            {
                scene::animation_controller_component& animation_controller_component = registry->get<
                    scene::animation_controller_component>(actor);
                const std::vector<glm::mat4>& final_bone_matrices = animation_controller_component.
                                                                    get_animation_controller()->
                                                                    get_final_bone_matrices();
                for (int i = 0; i < final_bone_matrices.size(); i++)
                {
                    shader->set_mat4("u_final_bone_matrices[" + std::to_string(i) + "]",
                                     final_bone_matrices[i]);
                }
            }
            shader->set_int("u_is_skeletal", has_animation_controller ? 1 : 0);
            
            shader->set_mat4("u_transform", transform->get_transform());

            renderer::submit_model(model);
        }
    }
}
