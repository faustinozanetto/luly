#pragma once

#include "scene/scene.h"

#include <yaml-cpp/yaml.h>

namespace luly::scene
{
    class scene_serializer
    {
    public:
        scene_serializer(const std::shared_ptr<scene>& scene);

        void serialize(const std::string& file_path);

    private:
        void serialize_actor(YAML::Emitter& out, const std::shared_ptr<scene_actor>& scene_actor);

        std::shared_ptr<scene> m_scene;
    };
}
