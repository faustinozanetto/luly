#pragma once

#include "scene/scene.h"
#include <memory>

namespace luly::renderer
{
    class shadow_manager
    {
    public:
        virtual  ~shadow_manager() = default;

        /* Virtuals */
        virtual void execute(const std::shared_ptr<scene::scene>& current_scene) = 0;

        /* Methods */
        void render_geometry(const std::shared_ptr<shader>& shader);
    };
}
