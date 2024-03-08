#pragma once

#include "scene/scene.h"

class animations_scene : public luly::scene::scene
{
public:
    animations_scene();
    ~animations_scene();

    void on_update(float delta_time) override;

private:
    void initialize();
    void create_animated_model();
};
